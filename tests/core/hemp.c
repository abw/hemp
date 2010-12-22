#include <hemp/test.h>


void test_hemp();


int main(
    int argc, char **argv, char **env
) {
    plan(11);
    test_hemp();
    return done();
}


void test_hemp_config(
    hemp_hemp   hemp,
    hemp_string name,
    hemp_string expect
) {
    hemp_value  value = hemp_config_value(hemp, name);

    if (hemp_is_string(value)) {
        hemp_string string = hemp_val_str(value);
        is(string, expect, name);
    }
    else if (hemp_is_defined(value)) {
        fail("Unexpected value for %s: %s", name, hemp_type_name(value));
    }
    else {
        fail("Config item not found: %s", name);
    }
}


void test_hemp() {
    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    HEMP_TRY;
        hemp_string version = hemp_version();
        ok( version, "got hemp version: %s", version );

        /* test the global configuration */
        hemp_value cfghemp = hemp_config_value(hemp, "hemp");
        ok( hemp_is_defined(cfghemp), "fetched hemp config" );

        test_hemp_config(hemp, "hemp.version",    version);
        test_hemp_config(hemp, "hemp.root",       HEMP_ROOT);
        test_hemp_config(hemp, "hemp.module_dir", HEMP_MODULE_DIR);
        test_hemp_config(hemp, "hemp.module_ext", HEMP_MODULE_EXT);

        /* add some extra configuration values from a hash */
        hemp_hash config = hemp_hash_new();
        hemp_hash_store_string(config, "foo",       "Hello");
        hemp_hash_store_string(config, "bar",       "World");
        hemp_hash_store_string(config, "hemp.root", "/some/where/else");

        /* add a nested hemp.root value to mask the global config value */
        hemp_hash hconf = hemp_hash_new();
        hemp_hash_store_hash(config, "hemp", hconf);
        hemp_hash_store_string(hconf, "root", "/some/where/else");

        hemp_configure(hemp, hemp_hash_val(config));

        test_hemp_config(hemp, "foo",           "Hello");
        test_hemp_config(hemp, "bar",           "World");
        test_hemp_config(hemp, "hemp.root",     "/some/where/else");
        test_hemp_config(hemp, "hemp.version",  version);

        hemp_hash_free(hconf);
        hemp_hash_free(config);

    HEMP_CATCH_ALL;
        fail("caught error: %s", hemp->error->message);

    HEMP_END;
    
    hemp_free(hemp);
}


