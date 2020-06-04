#include "skel.h"

int interfaces[ROUTER_NUM_INTERFACES];
struct route_table_entry *rtable;  // tabela de rutare 
int rtable_size;

struct arp_entry *arp_table;
int arp_table_len;

/*
 Returns a pointer (eg. &rtable[i]) to the best matching route
 for the given dest_ip. Or NULL if there is no matching route.
*/
struct route_table_entry *get_best_route(__u32 dest_ip) {
	/* TODO 1: Implement the function */
	uint32_t max = 0;
	int j = -1;;
	for (int i = 0; i < rtable_size; i++) {
		if ((rtable[i].mask & dest_ip) == rtable[i].prefix) {
			if (rtable[i].mask > max) {
				max = rtable[i].mask;
				j = i;
			}
				
		}
	}

	if (j != -1)
		return &rtable[j];
	return NULL;
}

/*
 Returns a pointer (eg. &arp_table[i]) to the best matching ARP entry.
 for the given dest_ip or NULL if there is no matching entry.
*/
struct arp_entry *get_arp_entry(__u32 ip) {
    /* TODO 2: Implement */
    // cand se descopera o ruta se trimite catre stiva mac
    // adresa mac = adresa fizica a device-ului
    // functia cicleaza prin arp_table si verifica daca cunoasre ip-ul trimis ca param => adresa MAC
   
	int index = -1;

    for (int i = 0; i < arp_table_len; i++) {
    	if (ip == arp_table[i].ip) {
    		index = i;
    	}
    }
    if (index != -1)
    	return &arp_table[index];
    return NULL;
}

int main(int argc, char *argv[])
{
	msg m;
	int rc;

	init();
	rtable = malloc(sizeof(struct route_table_entry) * 100);
	arp_table = malloc(sizeof(struct  arp_entry) * 100);
	DIE(rtable == NULL, "memory");
	rtable_size = read_rtable(rtable);
	//printf("%d\n", rtable[0].next_hop);
	parse_arp_table();
	/* Students will write code here */

	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");
		struct ether_header *eth_hdr = (struct ether_header *)m.payload;
		struct iphdr *ip_hdr = (struct iphdr *)(m.payload + sizeof(struct ether_header));
		/* TODO 3: Check the checksum */
		// ip_checksum intoarce int, daca ip_checksum nu e 0 se face drop-> continue
		if (ip_checksum(ip_hdr, sizeof(struct iphdr)) != 0) {
			printf("Wrong checksum\n"); 
			continue; // se duce la inceputul lui while-> nu se trimite pachetul 
		}
		/* TODO 4: Check TTL >= 1 */
		// TTL = time to leave - numar care la fiecare pas este decrementat, cand ajunge la 0 nu se face rutarea
		if (ip_hdr->ttl < 1) {
			printf("Time to live!\n");
			continue;
		}
		/* TODO 5: Find best matching route (using the function you wrote at TODO 1) */
		  //= malloc(sizeof(struct (route_table_entry))); 
		struct route_table_entry *todo5 = get_best_route(ip_hdr->daddr);
		if (todo5 == NULL) {
			printf("No router found!\n");
			continue;
		}
		/* TODO 6: Update TTL and recalculate the checksum */
		// check din iphdr - trebuie initializat cu 0, in acel camp se scrie rez lui ip_checksum 
		ip_hdr->ttl--;
		ip_hdr->check = 0;
		ip_hdr->check = ip_checksum(ip_hdr, sizeof(struct iphdr));
		/* TODO 7: Find matching ARP entry and update Ethernet addresses */
		struct  arp_entry *todo7 = get_arp_entry(ip_hdr->daddr);
		if (todo7 == NULL) {
			printf("No ARP entry found!\n");
			
			continue;
		}
		// arp mapeaza adresa mac de adresa ip
		memcpy(eth_hdr->ether_dhost, todo7->mac, sizeof(todo7->mac) + 1);
		/* TODO 8: Forward the pachet to best_route->interface */
		send_packet(todo5->interface, &m);
	}
}

