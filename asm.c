char *source = 
	"push 512 "
	"push 11 push 13 ADD "
	"push 4 SUB "
	"store "
	"push 512 load "
	"print "
	"halt "
;

char *operations[] = {
	"halt", "push", "load", "store", "add", "sub", "print",
};

void main()
{
}
