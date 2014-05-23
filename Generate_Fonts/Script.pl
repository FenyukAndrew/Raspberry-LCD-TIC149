#!/usr/bin/perl -w
    use strict;
    use warnings;

print "Hello world\n";

#my $number1 = 1.1;
#my $my_string1=qq[В этой строке можно использовать " '];

#my $scalar1 = 5;
#my $scalar2 = "Hello there!";

#print $number1;
#print $my_string1."\n";

#print "ZZZ\n" if (<STDIN> eq"\n");

#{{8, 5},{
#0x00,  /*  ........  */

#{{16, 14},{
#0x00,0x00,  /*  ................  */

#{{24, 21},{
#0x00,0x00,0x00,  /*  ........................  */
#0x00,0x00,0x00,  /*  ........................  */
#0x00,0x00,0x00,  /*  ........................  */

#{{32, 27},{
#0x00,0x00,0x00,0x00,  /*  ................................  */
#0x00,0x00,0x00,0x00,  /*  ................................  */

	open (FILE, '<', 'Tahoma_16_original.h') or die "Could not open Tahoma_16_original.h: $!";
    while (<FILE>) {
		#chomp;#удаление перевода строк

#Не работает никак фильтрация или скорее всего регулярные выражения
        #print $_ if ("^{{");# .. /^},{/
		if ("{{") { print $_}

	   #($name, $email, $phone) = split("\t");
	   #print "Name: $name\n";
       #print "Email: $email\n";
       #print "Phone: $phone\n";
       #print "---------\n";
    }
    close (FILE) or die "Could not close Tahoma_16_original.h: $!";



