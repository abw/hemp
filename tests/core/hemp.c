#include <hemp/test.h>


void test_hemp();
void test_hemp_text_config();


int main(
    int argc, char **argv, char **env
) {
    plan(17);
    test_hemp();
    test_hemp_text_config();
    return done();
}


void test_hemp_config(
    Hemp   hemp,
    HempString name,
    HempString expect
) {
    HempValue  value = hemp_config_get(hemp, name);
    HempString string;

    if (hemp_is_string(value)) {
        string = hemp_val_str(value);
        is(string, expect, name);
    }
    else if (hemp_is_text(value)) {
        string = hemp_val_text(value)->string;
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
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    HEMP_TRY;
        HempString version = hemp_version();
        ok( version, "got hemp version: %s", version );

        /* test the global configuration */
        HempValue cfghemp = hemp_config_get(hemp, "hemp");
        ok( hemp_is_defined(cfghemp), "fetched hemp config" );

        test_hemp_config(hemp, "hemp.version",    version);
        test_hemp_config(hemp, "hemp.dir",        HEMP_DIR);
        test_hemp_config(hemp, "hemp.module_dir", HEMP_MODULE_DIR);
        test_hemp_config(hemp, "hemp.module_ext", HEMP_MODULE_EXT);

        /* add some extra configuration values from a hash */
        HempHash config = hemp_hash_new();
        hemp_hash_store_string(config, "foo",       "Hello");
        hemp_hash_store_string(config, "bar",       "World");
        hemp_hash_store_string(config, "hemp.dir",  "/some/where/else");

        /* add a nested hemp.root value to mask the global config value */
        HempHash hconf = hemp_hash_new();
        hemp_hash_store_hash(config, "hemp", hconf);
        hemp_hash_store_string(hconf, "dir", "/some/where/else");

        /* configure the hemp object from the hash and check values */
        hemp_configure(hemp, hemp_hash_val(config));
        test_hemp_config(hemp, "foo",           "Hello");
        test_hemp_config(hemp, "bar",           "World");
        test_hemp_config(hemp, "hemp.dir",      "/some/where/else");
        test_hemp_config(hemp, "hemp.version",  version);

        /* read configuration from a file */
        hemp_language(hemp, "json");  // FIXME
        hemp_configure_from(hemp, "json", "file", HEMP_TEST_DIR "/data/config1.json");
        test_hemp_config(hemp, "hemp.dir",      "/dir/from/config1");
        test_hemp_config(hemp, "hemp.animal",   "badger");
        test_hemp_config(hemp, "extra.cheese",  "please");

        hemp_hash_free(hconf);
        hemp_hash_free(config);

    HEMP_CATCH_ALL;
        fail("caught error: %s", hemp->error->message);
        HempText error = hemp_error_text(hemp->error);
        printf("%s", error->string);
        hemp_text_free(error);

    HEMP_END;
    
    hemp_free(hemp);
}


void test_hemp_text_config() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    hemp_language(hemp, "json");

    HEMP_TRY;
        hemp_configure_from(
            hemp, "json", "text", 
            "{ hemp: { dir: '/else/where', path: ['here', '/there'] } }"
        );
        pass("configured from text");
        test_hemp_config(hemp, "hemp.dir",  "/else/where");
        //test_hemp_config(hemp, "hemp.path", "here/there");
        hemp_codec(hemp, "uri");

    HEMP_CATCH_ALL;
        fail("caught error: %s", hemp->error->message);
        HempText error = hemp_error_text(hemp->error);
        printf("%s", error->string);
        hemp_text_free(error);

    HEMP_END;
    
    hemp_free(hemp);
}