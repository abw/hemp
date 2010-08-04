#include <hemp/dialect.h>
#include <hemp/scanner.h>


hemp_dialect_p
hemp_dialect_tt3(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    hemp_dialect_p dialect = hemp_dialect_init(hemp, name);
    
    dialect->prepare = &hemp_dialect_tt3_prepare;
    dialect->scanner = &hemp_scan_text;
    dialect->cleanup = NULL; // &hemp_dialect_tt3_cleanup;
    
    return dialect;
}


hemp_template_p
hemp_dialect_tt3_prepare(
    hemp_template_p tmpl
    // TODO: options
) {
    debug_yellow("** XOOP! ** Running TT3 template prepare **\n");
    hemp_tagset_p tagset = tmpl->tagset;
    
    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "comment", 
            HEMP_INLINE,
            &hemp_scan_comment_tag,
            "[#", "#]"
        )
    );

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "control", 
            HEMP_INLINE,
            &hemp_scan_control_tag,
            "[?", "?]"
        )
    );

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "outline", 
            HEMP_OUTLINE,
            &hemp_scan_outline_tag,
            "%%", NULL
        )
    );

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            "inline", 
            HEMP_INLINE,
            &hemp_scan_inline_tag,
            "[%", "%]"
        )
    );

//    hemp_tagset_add_inline_tag(tagset, HempTagVariable);
//    hemp_tagset_add_inline_tag(tagset, HempTagEmbed);


    return tmpl;
}


void
hemp_dialect_tt3_cleanup(
    hemp_template_p tmpl
) {
    debug_yellow("** ZING! ** Running TT3 template cleanup **\n");
}


