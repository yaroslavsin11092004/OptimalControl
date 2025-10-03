#!/bin/bash
function run_server()
{
	alacritty -e ./release/$1 &
}
export DRI_PRIME=1
case "$1" in 
	"-a")
	run_server AdamServer.rl 
	;;	
	"-c")
	run_server CoreServer.rl 
	;;
	"-t")
	run_server TasksServer.rl 
	;;
	"-p")
	run_server ProxyServer.rl 
	;;
	"-as")
	run_server AdamServer.rl 
	run_server CoreServer.rl 
	run_server TasksServer.rl 
	run_server ProxyServer.rl
esac
