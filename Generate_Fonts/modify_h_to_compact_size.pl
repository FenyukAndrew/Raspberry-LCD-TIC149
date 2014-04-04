#!/usr/bin/perl -w
    use strict;
    use warnings;

print "Hello world\n";

my $number1 = 1.1;
my $my_string1=qq[В этой строке можно использовать " '];

my $scalar1 = 5;
my $scalar2 = "Hello there!";

print $number1;
print $my_string1."\n";

print "ZZZ\n" if (<STDIN> eq"\n");


#    open (FILE, '<', 'Tahoma_16_original.h') or die "Could not open Tahoma_16_original.h: $!";
#    while (<FILE>) {
#        print $_ if (/^{{/ .. /^},{/);
#    }
#    close (FILE) or die "Could not close Tahoma_16_original.h: $!";
