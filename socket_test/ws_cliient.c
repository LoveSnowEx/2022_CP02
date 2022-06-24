/*
* Copyright (C) 2022 Davidson Francis <davidsondfgl@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ws.h>

/**
 * @dir examples/ping
 * @brief Ping example directory
 *
 * @file ping.c
 * @brief Main file.
 */

/**
 * @brief Called when a client connects to the server.
 *
 * @param client Client connection.
 */
void onopen(ws_cli_conn_t *client)
{
	char *cli;
	cli = ws_getaddress(client);
	printf("Connection opened, addr: %s\n", cli);
	ws_sendframe_txt(client, "SEND\n"
							"destination:/queue/a\n"
							"content-type:text/plain\n"
							"welcome queue a\n"
							"^@");
}

/**
 * @brief Called when a client disconnects to the server.
 *
 * @param client Client connection.
 */
void onclose(ws_cli_conn_t *client)
{
	char *cli;
	cli = ws_getaddress(client);
	printf("Connection closed, addr: %s\n", cli);
}

/**
 * @brief Called when a client connects to the server.
 *
 * @param client Client connection.
 *
 * @param msg Received message.
 * @param size Message size (in bytes).
 * @param type Message type.
 */
void onmessage(ws_cli_conn_t *client,
	const unsigned char *msg, uint64_t size, int type)
{
	((void)client);
	((void)msg);
	((void)size);
	((void)type);

	
	char *cli;
	cli = ws_getaddress(client);
	printf("I receive a message: %s (%zu), from: %s\n", msg,
		size, cli);

	ws_sendframe_txt(client, "SEND\n"
							"destination:/queue/a\n"
							"content-type:text/plain\n"
							"hello queue a\n"
							"^@");
	sleep(2);
	ws_sendframe_txt(client, "SEND\n"
							"destination:/queue/a\n"
							"content-type:text/plain\n"
							"world queue a\n"
							"^@");
}

/**
 * @brief Main routine.
 */
int main(void)
{
	struct ws_events evs;
	evs.onopen    = &onopen;
	evs.onclose   = &onclose;
	evs.onmessage = &onmessage;
	ws_socket(&evs, 8080, 1, 1000);
	
	/*
	* Periodically send ping frames in the main thread
	* and aborts inactive connections.
	*/
	while (1)
	{
		/*
		* Sends a broadcast PING with 2-DELAY MS of tolerance, i.e:
		* the client can miss up to 2 PINGs messages.
		*
		* The 'timeout' is specified by the time between ws_ping()
		* calls. In this example, 10 seconds.
		*/
		// printf("Sending ping...\n");
		ws_ping(NULL, 2);

		/* Sleep 10 seconds. */
		sleep(1);
	}

	return (0);
}
