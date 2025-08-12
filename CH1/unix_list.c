#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
int main(){
  struct ifaddrs *addresses;
  /*
   * L I N K E D     L I S T
   * ifa_next -> pointer to next structure(ifaddrs)
   * ifa_name -> name of the network interface
   * ifa_flags -> SIOCGIFFLAGS
   * ifa_addr -> network address of the interface
   * ifa_netmask -> netmask of interface
   * ifu_broadaddr -> broadcast address 
   * ifu_dstaddr -> point-to-point destination address
   * getifaddrs() -> list of each network interface on the host 
   * freeifaddrs -> free
   * */
  if(getifaddrs(&addresses) == -1){ 
    printf("getifaddres call failed.\n");
    return -1;
  }
  
  struct ifaddrs *address = addresses; 
  while(address) {
    int family = address->ifa_addr->sa_family;
    if(family == AF_INET || family == AF_INET6){ // if ipv4 or IPV6
      printf("%s\t", address->ifa_name);
      printf("%s\t", family == AF_INET ? "IPV4" : "IPV6");
      char ap[100];
      const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
      getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
      /*
       * F U N C T I O N A L I T Y
       * socket address --> host and service name
       * ==========================================
      */
      /* P A R A M E T E R S
       * sockaddr -> in our case ifa_addr
       * socklen_t -> depends on ip version sizeof(sockaddr_in) or (sockaddr_in6)
       * host -> output string
       * hostlen -> sizeof(host)
       * serv -> 
       * servlen ->
       * flags ->
       *        NI_NAMEREQD -> return error if hostname != 1
       *        NI_DGRAM -> if service is dtagram based 
       *        NI_NOFQDN -> return only hostname part 
       *        NI_NUMERICHOST -> numeric form of the hostname
       *        NU_NUMERICSERV -> numeric form of the service address
       * */
      printf("\t%s\n", ap);
    }
    address = address->ifa_next;
  }
  freeifaddrs(addresses);
  return 0;
}
