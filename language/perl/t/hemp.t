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
    tests => 1,
    debug => 'Hemp',
    args  => \@ARGV;

use Hemp;
pass( 'Loaded Hemp' );

print STDERR "HEMP VERSION: ", Hemp::hemp_version, "\n";

