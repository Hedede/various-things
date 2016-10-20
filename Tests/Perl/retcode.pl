`sh ./retcode.sh`;
print $?;
if ( ($? >> 8) == 0 )
{
	print "success"
}
else
{
	print "error"
}
