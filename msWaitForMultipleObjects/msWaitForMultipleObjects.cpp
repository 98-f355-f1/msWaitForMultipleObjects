#include <stdio.h>
#include <windows.h>

#define NUM_EVENTS 3

HANDLE ghEvents[NUM_EVENTS];

DWORD WINAPI ThreadProc(LPVOID);

int main(int argc, char** argv)
{
	HANDLE hThread;
	DWORD i, dwEvent, dwThreadID;

	// create 2 events
	for (i = 0; i < NUM_EVENTS; i++) {
		ghEvents[i] = CreateEvent(
			NULL, 	// default security attributes
			FALSE,	// auto-reset event object
			FALSE,	// initial state is nonsignaled
			NULL);	// unnamed object	 

		if (ghEvents[i] == NULL) {
			printf("CreateThread failed\n", GetLastError());
			ExitProcess(0);
		}
	}

	// create a thread
	hThread = CreateThread(
		NULL,         // default security attributes
		0,            // default stack size
		(LPTHREAD_START_ROUTINE)ThreadProc,
		NULL,         // no thread function arguments
		0,            // default creation flags
		&dwThreadID); // receive thread identifier

	if (hThread == NULL) {
		printf("CreateThread failed\n", GetLastError());
		return(EXIT_FAILURE);
	}

	// Wait for the thread to signal one of the event objects
	dwEvent = WaitForMultipleObjects(
		NUM_EVENTS,	// number of events in queue
		ghEvents,		// the queue of objects
		FALSE,			// wait for any object
		5000				// delay in msecs
	);

	// return value dictates which event is signaled										 
	switch (dwEvent)
	{
		// ghEvents[0] was signaled
	case WAIT_OBJECT_0 + 0:
		// TODO: real handler here
		printf("First event was signaled\n");
		break;

	case WAIT_OBJECT_0 + 1:
		printf("Second event was signaled\n");
		break;

	case WAIT_OBJECT_0 + 2:
		printf("Third event was signaled\n");
		break;

	case WAIT_TIMEOUT:
		printf("Wait timed out\n");
		break;

	default:
		printf("Wait error: %d\n", GetLastError());
		ExitProcess(0);
	}

	// close event handles
	for (i = 0; i < NUM_EVENTS; i++)
		CloseHandle(ghEvents[i]);

	getchar(); // Hold terminal for debugging
	return (EXIT_SUCCESS);
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	// lpParam is not used in this example
	UNREFERENCED_PARAMETER(lpParam);

	if (!SetEvent(ghEvents[0]))
	{
		printf("Set Event failed (%d)\n", GetLastError());
		return(EXIT_FAILURE);
	}

	return(EXIT_SUCCESS);
}
