#========================================================================
#
# t/hemp.t
#
# Test the Hemp module.
#
# Written by Andy Wardley <abw@wardley.org>
#
# This is free software; you can redistribute it and/or modify it
# under the same terms as Perl itself.
#
#========================================================================

use lib qw( ./lib ../lib ../lib );
use Badger::Test
    tests => 6,
    debug => 'Hemp',
    args  => \@ARGV;

use Hemp;
pass( 'Loaded Hemp' );

print STDERR "HEMP VERSION: ", Hemp::hemp_version, "\n";

test_hemp();

sub test_hemp() {
    # create new Hemp interpreter
    my $hemp = Hemp->new();
    ok( $hemp, "Got hemp object: $hemp\n" );

    # load the tt3 language pack
    $hemp->language("tt3");

    # create a template
    my $template = $hemp->template( tt3 => text => "Hello [% name %]!" );
    ok( $template, "Got hemp template: $template\n" );

    # create a runtime template
    my $context = $hemp->context;
    ok( $context, "Got hemp context: $context\n" );

    # define a template variable
    $context->set( name => "World" );

    # render the template
    my $output = $template->render($context);
    ok( $output, "Processed template: $output" );
    is( $output, "Hello World!", "got correct output" );

    print STDERR "OUTPUT: $output\n";

    # hemp and context should be automatically freed via DESTROY methods
}


