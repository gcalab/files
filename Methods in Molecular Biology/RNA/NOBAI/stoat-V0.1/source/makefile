
#****************************************************************************#
#                                                                            #
# 		This is the STOAT-V.0.1 make-file. Just do the               #
#		adjustments and type make.                                   #
#                                                                            #
#								  STOAT 2007 #
#****************************************************************************#


CC	= g++ 
CFLAGS 	=  -Wall -O -ansic -Wno-deprecated
OBJS	= main.o  


stoat: 	$(OBJS) 
	$(CC) $(CFLAGS) -o $@  $(OBJS)

clean:
	rm  *.o a.out *.*~ *~

install:
	mv ./stoat ../bin


