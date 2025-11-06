#ifndef DIFF_EVOLUTION_H
#define DIFF_EVOLUTION_H
#include <vector>
#include <thread>
#include <algorithm>
#include <atomic>
#include <absl/random/random.h>
#include <absl/container/fixed_array.h>
#include <absl/synchronization/mutex.h>
#include <absl/types/span.h>
#include <absl/functional/any_invocable.h>

template<size_t max_dims = 64>
class DifferentialEvolution {
	private:
		using Individual = absl::FixedArray<double, max_dims>;
		using Population = std::vector<Individual>;
		using ObjectiveFn = absl::AnyInvocable<double(absl::Span<const double>)>;

		ObjectiveFn objective;
		std::vector<std::pair<double, double>> bounds;
		int population_size;
		double F, CR;
		int num_threads;
	public:
		struct Result {
			std::vector<double> best_solution;
			double best_fitness;
			std::vector<double> history;
			size_t evaluations;
		};
		DifferentialEvolution(ObjectiveFn objective_, 
				const std::vector<std::pair<double, double>>& bounds_,
				int pop_size = 50, double F_ = 0.8, double CR_ = 0.9, int threads = 4):
			objective(std::move(objective_)), bounds(bounds_), population_size(pop_size),
			F(F_), CR(CR_), num_threads(std::min(threads, pop_size)) {}
		Result optimize(int max_iterations) {
			const int dim = bounds.size();
			auto [population, fitness] = initialize(dim);
			int best_idx = find_best_index(fitness);
			Individual best_solution = create_copy(population[best_idx]);
			double best_fitness = fitness[best_idx];
			std::vector<double> history;
			history.reserve(max_iterations);
			std::atomic<size_t> total_evaluations = population_size;
			absl::Mutex best_mutex;
			for (int iter = 0; iter < max_iterations; iter++) {
				selection_phase(population, fitness, total_evaluations, dim);
				update_best_solution(population, fitness, best_solution, best_fitness, best_mutex, dim);
				history.push_back(best_fitness);
			}
			return {
				std::vector<double>(best_solution.begin(), best_solution.end()),
				best_fitness,
				std::move(history),
				total_evaluations.load()
			};
		}
	private:
		Individual create_copy(const Individual& src) {
			Individual cp(src.size());
			std::copy(src.begin(), src.end(), cp.begin());
			return cp;
		}
		void update_individual(Individual& dest, const Individual& src) {
			std::copy(src.begin(), src.end(), dest.begin());
		}
		std::pair<Population, std::vector<double>> initialize(int dim) {
			Population population;
			population.reserve(population_size);
			std::vector<double> fitness(population_size);
			for (int i = 0; i < population_size; i++)
				population.emplace_back(dim);
			std::vector<std::thread> workers;
			int chunk_size = population_size / num_threads;
			for (int t = 0; t < num_threads; t++) {
				workers.emplace_back([&, t, dim]() {
					int start = t * chunk_size;
					int end = (t == num_threads - 1) ? population_size : start + chunk_size;
					thread_local absl::BitGen bitgen;
					for (int i = start;i < end; i++) {
						for (int j = 0; j < dim; j++)
							population[i][j] = absl::Uniform(bitgen, bounds[j].first, bounds[j].second);
						fitness[i] = objective(absl::MakeSpan(population[i]));
					}
				});
			}
			for (auto& th : workers) 
				th.join();
			return { std::move(population), std::move(fitness) };
		}
		void selection_phase(Population& population, std::vector<double>& fitness, std::atomic<size_t>& total_evaluations, int dim) {
			std::vector<std::thread> workers;
			int chunk_size = population_size / num_threads;
			for (int t = 0; t < num_threads; t++) {
				workers.emplace_back([&, t, dim]() {
					int start = t * chunk_size;
					int end = (t == num_threads - 1) ? population_size : start + chunk_size;
					thread_local absl::BitGen bitgen;
					for (int i = start; i < end; i++) {
						auto trial = create_trial(population[i], population, i, bitgen, dim);
						double trial_fitness = objective(absl::MakeSpan(trial));
						if (trial_fitness < fitness[i]) {
							update_individual(population[i], trial);
							fitness[i] = trial_fitness;
						}
					}
				});
			}
			for (auto& th : workers) {
				th.join();
			}
			total_evaluations.fetch_add(population_size, std::memory_order_relaxed);
		}
		Individual create_trial(const Individual& target, const Population& population, int current_idx, absl::BitGen& bitgen, int dim) {
			const int pop_size = population.size();
			Individual trial = create_copy(target);
			int a,b,c;
			select_indeces(current_idx, pop_size, a,b,c, bitgen);
			const int R = absl::Uniform(bitgen, 0, dim);
			bool mutated = false;
			for (int j = 0; j < dim; j++) {
				if (j == R || absl::Uniform(bitgen, 0.0, 1.0) < CR) {
					trial[j] = population[a][j] + F * (population[b][j] - population[c][j]);
					trial[j] = std::clamp(trial[j], bounds[j].first, bounds[j].second);
					mutated = true;
				}
			}
			if (!mutated) {
				const int j = absl::Uniform(bitgen, 0, dim);
				trial[j] = population[a][j] + F * (population[b][j] - population[c][j]);
				trial[j] = std::clamp(trial[j], bounds[j].first, bounds[j].second);
			}
			return trial;
		}
		void select_indeces(int current_idx, int pop_size, int& a, int& b, int& c, absl::BitGen& bitgen) {
			a = absl::Uniform(bitgen,0,pop_size);
			b = absl::Uniform(bitgen,0,pop_size);
			c = absl::Uniform(bitgen,0,pop_size);
			if (a == current_idx) a = (a + 1) % pop_size;
			if (b == current_idx || b == a) b = (b + 1) % pop_size;
			if (c == current_idx || c == a || c == b) c = (c + 1) % pop_size;
		}
		int find_best_index(const std::vector<double>& fitness) {
			return std::distance(fitness.begin(), std::min_element(fitness.begin(), fitness.end()));
		}
		void update_best_solution(const Population& population, const std::vector<double>& fitness, Individual& best_solution, double& best_fitness, absl::Mutex& best_mutex, int dim) {
			std::vector<double> local_bests(num_threads, best_fitness);
			std::vector<Individual> local_solution(num_threads, create_copy(best_solution));
			std::vector<std::thread> workers;
			int chunk_size = population_size / num_threads;
			for (int t = 0; t < num_threads; t++) {
				workers.emplace_back([&, t, dim]() {
					int start = t * chunk_size;
					int end = (t == num_threads - 1) ? population_size : start + chunk_size;
					for (int i = start; i < end; i++) {
						if (fitness[i] < local_bests[t]) {
							local_bests[t] = fitness[i];
							update_individual(local_solution[t], population[i]);
						}
					}
				});
			}
			for (auto& th : workers)
				th.join();
			auto min_it = std::min_element(local_bests.begin(), local_bests.end());
			if (min_it != local_bests.end() && *min_it < best_fitness) {
				absl::MutexLock lock(&best_mutex);
				if (*min_it < best_fitness) {
					best_fitness = *min_it;
					update_individual(best_solution, local_solution[std::distance(local_bests.begin(), min_it)]);
				}
			}
		}
};
#endif
