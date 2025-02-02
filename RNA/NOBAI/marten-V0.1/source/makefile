#****************************************************************************#
#	                                                                     #
#                                                                            #
# 		This is the MARTEN-2.1v make-file. Just do the               #
#		adjustments and type make.                                   #
#                                                                            #
#								 MARTEN 2007 #
#****************************************************************************#


CC	= g++ 
CFLAGS 	=  -Wall -O -ansic -Wno-deprecated
OBJS	= main.o  


marten: $(OBJS) 
	$(CC) $(CFLAGS) -o $@  $(OBJS)

clean:
	rm  *.o a.out *.*~ *~

install:
	mv ./marten ../bin


