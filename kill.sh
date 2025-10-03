#!/bin/bash
case "$1" in 
	"-c")
	pkill -f "CoreServer.rl"
	;;
	"-a")
	pkill -f "AdamServer.rl"
	;;
	"-t")
	pkill -f "TasksServer.rl"
	;;
	"-p")
	pkill -f "ProxyServer.rl"
	;;
"-as")
	pkill -f "CoreServer.rl"
	pkill -f "AdamServer.rl"
	pkill -f "TasksServer.rl"
	pkill -f "ProxyServer.rl"
	;;
esac
