#!/bin/bash

function createUser {
    echo -n "Enter username : "
	read username
	echo -n "Enter password : " 
	read password
	sudo useradd -m -s /bin/bash "$username"
	echo -e "$password\n$password" | sudo passwd "$username"
	sudo usermod -aG sudo "$username"
	sudo cut -d: -f1 /etc/passwd
    echo "User $username is created and administrator privileges are assigned"
}

function displayApps {
    dpkg --get-selections
}

function installApp {
    echo "Enter the name for the application you want to install : "
    read name
    sudo apt-get install "$name"
}

function setNetwork {
    # Extract network information using ifconfig
    ip_addr=$(ifconfig eth0 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
    mask=$(ifconfig eth0 | grep -oP '(?<=netmask\s)\d+(\.\d+){3}')
    default_route=$(ip route | grep default | awk '{print $3}')
    dns_server=$(grep nameserver /etc/resolv.conf | awk '{print $2}')
  
    echo "IP Address: $ip_addr"
    echo "Subnet Mask: $mask"
    echo "Default Gateway: $default_route"
    echo "DNS Server: $dns_server"
  
}

function showHelp {
    echo "Options:"
    echo "  -uc     Create a new user with administrator privileges"
    echo "  -ld     List installed applications"
    echo "  -ins    Install an application"
    echo "  -ipcon  Set network configuration"
    echo "  -h      Show help message"
}

echo "20k-1724-Ashar Khalil"

while [[ $# -gt 0 ]]; do
    case "$1" in
        -uc)
            createUser
            ;;
        -ld)
            displayApps
            ;;
        -ins)
            installApp
            ;;
        -ipcon)
            setNetwork
            ;;
        -h)
            showHelp
            ;;
        *)
            echo "Unknown option: $1"
            ;;
    esac
    shift
done
