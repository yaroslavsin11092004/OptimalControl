#!/bin/bash
alacritty -e ./release/AdamServer.rl & 
alacritty -e ./release/CoreServer.rl & 
alacritty -e ./release/ProxyServer.rl &
alacritty -e ./release/TasksServer.rl &
