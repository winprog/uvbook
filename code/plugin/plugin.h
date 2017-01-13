#ifndef UVBOOK_PLUGIN_SYSTEM
#define UVBOOK_PLUGIN_SYSTEM

#include <stdio.h>

// Plugin authors should use this to register their plugins with mfp.
void mfp_register(const char *name) {
	fprintf(stderr, "Registered plugin \"%s\"\n", name);
}

#endif
