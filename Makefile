all: TCPStudentClient TCPTeacherServer
TCPStudentClient: TCPStudentClient.c DieWithError.c
	gcc TCPStudentClient.c DieWithError.c -o TCPStudentClient
TCPTeacherServer: TCPTeacherServer.c DieWithError.c
	gcc TCPTeacherServer.c DieWithError.c HandleTCPClient.c -o TCPTeacherServer
