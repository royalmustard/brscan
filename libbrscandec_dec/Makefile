all: libbrscandec.so.1.0.0

ghidra_decomp.o: ghidra_decomp.c ghidra_decomp.h
	gcc -fPIC -c $< -o $@

brother_scandec.o: brother_scandec.c brother_scandec.h
	gcc -fPIC -c $< -o $@

# brother_bugchk.o: brother_bugchk.c brother_bugchk.h
# 	gcc -fPIC -c $< -o $@

# brother_chreso.o: brother_chreso.c brother_chreso.h
# 	gcc -fPIC -c $< -o $@

libbrscandec.so.1.0.0: brother_scandec.o 
	gcc -shared $^ -o $@

clean:
	rm -rf *.o libbrscandec.so.1.0.0

