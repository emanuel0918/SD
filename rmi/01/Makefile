IP=localhost
PORT=54321
all:	
	javac *.java
run:
	java -Djava.security.policy=cliente.permisos ClienteEco $(IP) $(PORT) hola
server:
	java -Djava.security.policy=servidor.permisos ServidorEco $(PORT)
cliente:
	java -Djava.security.policy=cliente.permisos ClienteEco $(IP) $(PORT) 
clean:
	rm *.class*
