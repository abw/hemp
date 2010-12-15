perl -p -e 's/\#(undef|define)\s+(HEMP_)?([A-Z]\w+)/#$1 HEMP_$3/; s/\bHEMP_VERSION\b/HEMP__VERSION/' include/config.h > include/hemp/config.h

