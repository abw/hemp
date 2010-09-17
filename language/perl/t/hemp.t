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
    tests => 7,
    debug => 'Hemp',
    args  => \@ARGV;

use Hemp;
pass( 'Loaded Hemp' );

print STDERR "HEMP VERSION: ", Hemp::hemp_version, "\n";

package Foo;
use Badger::Class
    base    => 'Badger::Base',
    as_text => \&text;

sub text {
    return "THIS IS A FOO OBJECT\n";
}

package main;

test_hemp();

sub test_hemp() {
    # create new Hemp interpreter
    my $hemp = Hemp->new();
    ok( $hemp, "Got hemp object: $hemp\n" );

    # load the tt3 language pack
    $hemp->language("tt3");

    # create a template
    my $template = $hemp->template( tt3 => text => "Hello [% name %]! [% n + 20 %]" );
    ok( $template, "Got hemp template: $template\n" );

    # create a runtime template
    my $context = $hemp->context;
    ok( $context, "Got hemp context: $context\n" );

    my $items  = ['foo', 10, 3.14];
    my $empty  = [ ];
    my $hash   = { a => 10, b => 20 };
    my $nohash = { };
    my $object = Foo->new();
    print "Foo: $object\n";

    # define a template variable
    $context->set( name   => "World" );
    $context->set( n      => 400 );
    $context->set( items  => $items );
    $context->set( empty  => $empty );
    $context->set( hash   => $hash );
    $context->set( nohash => $nohash );
    $context->set( object => $object );

    # render the template
    my $output = $template->render($context);
    ok( $output, "Processed template: $output" );
    is( $output, "Hello World! 420", "got correct output" );
    print STDERR "OUTPUT: $output\n";

    $template = $hemp->template( tt3 => text => "World:[% name.length %] [% n %]:[% n.length %]" );
    ok( $template, "Got hemp template: $template\n" );
    print STDERR "OUTPUT: ", $template->render($context), "\n";

#    $template = $hemp->template( tt3 => text => "World:[% name.length %] [% n %]:[% n.length %]" );
#    ok( $template, "Got hemp template: $template\n" );

#    print STDERR 
#        "OUTPUT: ", 
#        $hemp
#            ->template( tt3 => text => 
#"hash length: [% hash.length %] nohash: [% nohash.length %]  items: [% items.length %]  empty: [% empty.length %]" )
#">>>> [% object %]" )
#            ->render($context),
#        "\n";



    # hemp and context should be automatically freed via DESTROY methods
}



