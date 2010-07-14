#============================================================= -*-perl-*-
#
# t/hemp.t
#
# Test the Perl interface to the hemp template library.
#
# Written by Andy Wardley <abw@wardley.org>
#
# This is free software; you can redistribute it and/or modify it
# under the same terms as Perl itself.
#
#========================================================================

use strict;
use warnings;

use lib qw( ./lib ./blib ./blib/arch ../lib ../blib ../blib/arch );
use Badger::Test 
    tests => 3,
    args  => \@ARGV;

my ($pkg, $obj);


use hemp;
pass( "Loaded hemp library" );

my $hemp = hemp::hemp_init();
ok( $hemp, "created hemp object: $hemp");

hemp::hemp_free($hemp);
pass("freed hemp object");


__END__
# Anything more advanced than that dumps core.  But I'm hardly surprised
# given that I've just dumped everything into SWIG using all the defaults


my $source = hemp::hemp_source( text => "Hello World" );
ok( $source, "got source" );

my $text = hemp::hemp_render( text => 'Hello World' );

my $tt3 = hemp::hemp_dialect_tt3();
ok(1, "HEHEHEH");
ok( $tt3, "got the tt3 dialect: $tt3");

my $tmpl = hemp::hemp_template_init( text => "The answer is [% 42 %]");
ok( $tmpl, "created template object: $tmpl" );

my $text = hemp::hemp_template_render($tmpl);
ok( $text, "created template output: $text" );


__END__

# Local Variables:
# mode: perl
# perl-indent-level: 4
# indent-tabs-mode: nil
# End:
#
# vim: expandtab shiftwidth=4:
