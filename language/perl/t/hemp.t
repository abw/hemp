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

use Debug::FaultAutoBT;
Debug::FaultAutoBT->new( 
    verbose  => 1,
    dir      => "/Users/abw/tmp",
    debugger => "gdb",
)->ready;

use Hemp;
pass( 'Loaded Hemp' );

print STDERR "HEMP VERSION: ", Hemp::hemp_version, "\n";

package Foo;
use Badger::Class
    base    => 'Badger::Base',
    as_text => \&text;

sub text {
    return "THIS IS A FOO OBJECT";
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
    my $hash   = { a => 10, b => "twenty", c => Foo->new };
    my $nohash = { };
    my $object = Foo->new();
    my $code   = sub { warn "in Perl subroutine\n"; return "Returned from Perl code" };
#    print "Foo: $object\n";

    # define a template variable
    $context->set( name   => "World" );
    $context->set( n      => 400 );
    $context->set( items  => $items );
    $context->set( empty  => $empty );
    $context->set( hash   => $hash );
    $context->set( nohash => $nohash );
    $context->set( object => $object );
    $context->set( plcode => $code );

    # render the template
#    my $output = $template->render($context);
#    ok( $output, "Processed template: $output" );
#    is( $output, "Hello World! 420", "got correct output" );
#    print STDERR "OUTPUT: $output\n";

    $template = $hemp->template( 
#        tt3 => text => "hash.a:[% hash.a %]  hash.b:[% hash.b %]  hash.c:[% hash.c %]" 
        tt3 => text => "code: [% plcode(n) %]" 
    );
    ok( $template, "Got hemp template: $template\n" );
    my $r = $template->render($context);
    my $t = $r->text;
    print "rendered: ", $r->text, "\n";
    print "autostring: $r\n";

#    print STDERR "OUTPUT: $r\n"; 

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



