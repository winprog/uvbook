#include <stdio.h>
#include <uv.h>

int main() {
    char buf[512];
    uv_interface_address_t *info;
    int count, i;

    uv_interface_addresses(&info, &count);
    i = count;

    printf("Number of interfaces: %d\n", count);
    while (i--) {
        uv_interface_address_t addr = info[i];

        printf("Name: %s\n", addr.name);
        printf("Internal? %s\n", addr.is_internal ? "Yes" : "No");
        
        if (addr.address.address4.sin_family == AF_INET) {
            uv_ip4_name(&addr.address.address4, buf, sizeof(buf));
            printf("IPv4 address: %s\n", buf);
        }
        else if (addr.address.address4.sin_family == AF_INET6) {
            uv_ip6_name(&addr.address.address6, buf, sizeof(buf));
            printf("IPv6 address: %s\n", buf);
        }

        printf("\n");
    }

    uv_free_interface_addresses(info, count);
    return 0;
}
