#!/bin/bash
function createUser
{
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
function displayapps
{
	dpkg --get-selections
}

function installApp
{
	echo "Enter the name for the application you want to install : "
	read name
	sudo apt-get install $name
}
function setNetwork 
{
  # reseting the network cofniguration 
  sudo ip addr del 10.0.0.1/24 dev ens33 2>/dev/null
  sudo ip route del default via 10.0.0.254 2>/dev/null
  sudo rm /etc/resolv.conf 2>/dev/null
  # updating the network configuration 
  # /24 here means tha subnet mask 0f 255.255.255.0 
  sudo ip addr add 10.0.0.1/24 dev ens33
  sudo ip route add default via 10.0.0.254 dev ens33
  echo "nameserver 8.8.8.8" | sudo tee /etc/resolv.conf >/dev/null
  
  # script to check if the network configuration changed or not
  ip_addr=$(ip addr show ens33 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
  default_route=$(ip route | grep default | awk '{print $3}')
  dns_server=$(grep nameserver /etc/resolv.conf | awk '{print $2}')
  mask=$(ip -o -4 addr show dev ens33 | awk '{print $4}')
  
  echo gateway = $ip_addr
  echo mask = $mask
  echo default_route = $default_route
  echo DNS = $dns_server
}
function showhelp
{
  echo "Options:"
  echo "  -uc     Create a new user with administrator privileges"
  echo "  -ld     List installed applications"
  echo "  -ins    Install Dropbox"
  echo "  -ipcon  Set network configuration"
  echo "  -h      Show help message"
}
echo "20k-1724-Ashar Khalil"
while [[ $# -gt 0 ]]; 
do
  case "$1" in
    -uc)
      createUser
      ;;
    -ld)
      displayapps
      ;;
    -ins)
      installApp
      ;;
    -ipcon)
      setNetwork
      ;;
    -h)
      showhelp
      ;;
    *)
      echo "Unknown option: $1"
      ;;
  esac
  shift
done
