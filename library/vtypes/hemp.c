#include "hemp.h"

hemp_text_t     hemp_value_text_text(hemp_value_t, hemp_context_t, hemp_text_t);
hemp_value_t    hemp_value_text_dot(hemp_value_t, hemp_context_t, hemp_cstr_t);
void            hemp_value_text_init(hemp_value_t);
void            hemp_value_text_wipe(hemp_value_t);

hemp_text_t     hemp_value_list_text(hemp_value_t, hemp_context_t, hemp_text_t);
hemp_value_t    hemp_value_list_dot(hemp_value_t, hemp_context_t, hemp_cstr_t);
hemp_text_t     hemp_value_hash_text(hemp_value_t, hemp_context_t, hemp_text_t);
hemp_value_t    hemp_value_hash_dot(hemp_value_t, hemp_context_t, hemp_cstr_t);



/*--------------------------------------------------------------------------
 * prepare vtypes entries for default hemp value types
 *--------------------------------------------------------------------------*/

void hemp_hub_prepare_vtypes_hemp(
    hemp_hub_t hub
) {
    hemp_vtypes_t vtypes = hemp_hub_vtypes(hub, HEMP_HEMP);
    hemp_vtype_t  vtype;
    
    if (vtypes) 
        hemp_fatal("hub already has %s value typeset installed", HEMP_HEMP);
    
    vtypes = hemp_vtypes_init(HEMP_HEMP);
    
    vtypes->text = vtype = hemp_vtype_init(vtypes, "text");
    vtype->init  = &hemp_value_text_init;
    vtype->wipe  = &hemp_value_text_wipe;
    vtype->text  = &hemp_value_text_text;
    vtype->dot   = &hemp_value_text_dot;

    vtypes->list = vtype = hemp_vtype_init(vtypes, "list");
    vtype->text  = &hemp_value_list_text;
    vtype->dot   = &hemp_value_list_dot;

    vtypes->hash = vtype = hemp_vtype_init(vtypes, "hash");
    vtype->text  = &hemp_value_hash_text;
    vtype->dot   = &hemp_value_hash_dot;
    
    hemp_hub_add_vtypes(hub, vtypes);
}




/*--------------------------------------------------------------------------
 * text methods
 *--------------------------------------------------------------------------*/

void
hemp_value_text_init(
    hemp_value_t    value
) {
    hemp_data_t data = value->data;
    hemp_text_t text = data.text;
    
    hemp_debug("text:init [%s]\n", text->string);
}

void
hemp_value_text_wipe(
    hemp_value_t    value
) {
    hemp_debug("text:wipe [%s]\n", value->data.text->string);
}

hemp_text_t
hemp_value_text_text(
    hemp_value_t    value,
    hemp_context_t  context,
    hemp_text_t     output
) {
    hemp_debug("text:text\n");
}


hemp_value_t
hemp_value_text_dot(
    hemp_value_t    value,
    hemp_context_t  context,
    hemp_cstr_t     name
) {
    hemp_debug("text:dot(%s)\n", name);
}


/*--------------------------------------------------------------------------
 * list methods
 *--------------------------------------------------------------------------*/

hemp_text_t
hemp_value_list_text(
    hemp_value_t    value,
    hemp_context_t  context,
    hemp_text_t     output
) {
    hemp_debug("list:text\n");
}

hemp_value_t
hemp_value_list_dot(
    hemp_value_t    value,
    hemp_context_t  context,
    hemp_cstr_t     name
) {
    hemp_debug("list:dot(%s)\n", name);
}


/*--------------------------------------------------------------------------
 * hash methods
 *--------------------------------------------------------------------------*/

hemp_text_t
hemp_value_hash_text(
    hemp_value_t    value,
    hemp_context_t  context,
    hemp_text_t     output
) {
    hemp_debug("hash:text\n");
}

hemp_value_t
hemp_value_hash_dot(
    hemp_value_t    value,
    hemp_context_t  context,
    hemp_cstr_t     name
) {
    hemp_value_t result = (hemp_value_t) hemp_hash_fetch(
        value->data.hash, name
    );
    hemp_debug("looked up\n");
    
    if (result) {
        hemp_debug("found '%s' in hash\n", name);
    }
    else {
        hemp_debug("missing '%s' in hash\n", name);
    }
    return result;
}
