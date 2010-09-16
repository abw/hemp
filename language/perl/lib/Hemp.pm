package Hemp;

use 5.008;
use Badger;
require XSLoader;

our $VERSION = 0.01;

XSLoader::load('Hemp', $VERSION);

1;

__END__

=head1 NAME

Hemp - Hemp weaves data into text

=head1 SYNOPSIS

    # NOTE: This is all still a bit clumsy at the moment but will improve
    
    # load the Hemp module and library
    use Hemp;
    
    # create a Hemp interpreter
    my $hemp = Hemp->new;               # TODO: config params
    
    # load the TT3 language pack (what little there is of it)
    $hemp->language("tt3");

    # create a TT3 template from a text string
    my $template = $hemp->template(
        'tt3',                          # dialect
        text => "Hello [% name %]!"     # source
    )
    
    # create a runtime context
    my $context = $hemp->context;
    
    # define a template variable
    $context->set( name => "World" );   # NOTE: only scalars at present
    
    # render the template in the context
    my $output = $template->render($context);
    
    # sit back and relax
    print $output;              # Hello World!

=head1 DESCRIPTION

Hemp weaves data into text.  It implements a parsing and processing 
framework for template languages and mini-markup languages like POD,
Markdown, etc.  Or rather, it will do, when it's finished.

As of September 2010, Hemp is just about advanced enough to process very
simple templates (think "Hello [% name %]!"). 

This module is a first attempt at providing a Perl wrapper around the Hemp C
library.  It is still at the proof-of-concept stage and is most definitely 
I<not> yet suitable for any kind of serious work.

=head1 AUTHOR

Andy Wardley, E<lt>abw@wardley.orgE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2010 by Andy Wardley

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.9 or,
at your option, any later version of Perl 5 you may have available.

=cut
