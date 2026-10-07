#include <ibus.h>
#include <stdio.h>
#include <stdlib.h>

extern GType mongol_engine_get_type(void);

static void ibus_disconnected_cb(IBusBus *bus, gpointer user_data) {
    ibus_quit();
}

int main(int argc, char **argv) {
    ibus_init();

    IBusBus *bus = ibus_bus_new();
    g_signal_connect(bus, "disconnected", G_CALLBACK(ibus_disconnected_cb), NULL);

    IBusFactory *factory = ibus_factory_new(ibus_bus_get_connection(bus));
    
    // "mongol" нэртэй хөдөлгүүрийг системд бүртгэх
    ibus_factory_add_engine(factory, "mongol", mongol_engine_get_type());

    if (argc > 1 && g_strcmp0(argv[1], "--ibus") == 0) {
        ibus_bus_request_name(bus, "org.freedesktop.IBus.Mongol", 0);
    }

    ibus_main();
    return 0;
}