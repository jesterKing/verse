/*
 * $Id: t_node_destroy.c 1267 2012-07-23 19:10:28Z jiri $
 *
 * ***** BEGIN BSD LICENSE BLOCK *****
 *
 * Copyright (c) 2009-2011, Jiri Hnidek
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***** END BSD LICENSE BLOCK *****
 *
 * Authors: Jiri Hnidek <jiri.hnidek@tul.cz>
 *
 */

#include <check.h>

#include "v_common.h"
#include "v_commands.h"
#include "v_node_commands.h"
#include "v_in_queue.h"
#include "v_out_queue.h"

#define CHUNK_NUM 4

START_TEST ( test_Node_Destroy_create )
{
	struct Generic_Cmd *node_destroy = NULL;
	const uint32 node_id = 65538;

	node_destroy = v_node_destroy_create(node_id);

	fail_unless( node_destroy != NULL,
			"Node_Destroy create failed");
	fail_unless( node_destroy->id == CMD_NODE_DESTROY,
			"Node_Destroy OpCode: %d != %d", node_destroy->id, CMD_NODE_DESTROY);

	fail_unless( UINT32(node_destroy->data[0]) == node_id,
			"Node_Destroy Node_ID: %d != %d", UINT32(node_destroy->data[0]), node_id);

	v_cmd_destroy(&node_destroy);

	fail_unless( node_destroy == NULL,
			"Node_Destroy destroy failed");
}
END_TEST

START_TEST( test_Node_Destroy_in_queue )
{
	struct VInQueue *in_queue = v_in_queue_create();
	struct Generic_Cmd *_node_destroy, *node_destroy = NULL;
	uint32 node_id;
	int i;

	/* Push commands to the queue */
	for(i=0; i<CHUNK_NUM; i++) {
		node_id = 10000+i;

		node_destroy = v_node_destroy_create(node_id);
		v_in_queue_push(in_queue, node_destroy);
	}

	/* Pop commands from the queue */
	for(i=0; i<CHUNK_NUM; i++) {
		node_id = 10000+i;

		_node_destroy = v_in_queue_pop(in_queue);

		fail_unless( _node_destroy != NULL,
				"Node_Destroy create failed");
		fail_unless( _node_destroy->id == CMD_NODE_DESTROY,
				"Node_Destroy OpCode: %d != %d", _node_destroy->id, CMD_NODE_DESTROY);

		fail_unless( UINT32(_node_destroy->data[0]) == node_id,
				"Node_Destroy Node_ID: %d != %d", UINT32(_node_destroy->data[0]), node_id);

		v_cmd_destroy(&_node_destroy);

		fail_unless( _node_destroy == NULL,
				"Node_Destroy destroy failed");
	}

	v_in_queue_destroy(&in_queue);
}
END_TEST

START_TEST( test_Node_Destroy_out_queue )
{
	struct VOutQueue *out_queue = v_out_queue_create();
	struct Generic_Cmd *_node_destroy, *node_destroy = NULL;
	uint32 node_id;
	uint16 count, len;
	int8 share;
	int i;

	/* Push commands to the queue */
	for(i=0; i<CHUNK_NUM; i++) {
		node_id = 10000+i;

		node_destroy = v_node_destroy_create(node_id);
		v_out_queue_push_tail(out_queue, VRS_DEFAULT_PRIORITY, node_destroy);
	}

	/* Pop commands from the queue */
	for(i=0; i<CHUNK_NUM; i++) {
		node_id = 10000+i;
		count = 0;
		share = 0;
		len = 65535;

		_node_destroy = v_out_queue_pop(out_queue, VRS_DEFAULT_PRIORITY, &count, &share, &len);

		fail_unless( _node_destroy != NULL,
				"Node_Destroy create failed");
		fail_unless( _node_destroy->id == CMD_NODE_DESTROY,
				"Node_Destroy OpCode: %d != %d", _node_destroy->id, CMD_NODE_DESTROY);

		fail_unless( UINT32(_node_destroy->data[0]) == node_id,
				"Node_Destroy Node_ID: %d != %d", UINT32(_node_destroy->data[0]), node_id);

		v_cmd_destroy(&_node_destroy);

		fail_unless( _node_destroy == NULL,
				"Node_Destroy destroy failed");
	}

	v_out_queue_destroy(&out_queue);
}
END_TEST

START_TEST( test_Node_Destroy_pack_unpack )
{
	struct VOutQueue *out_queue = v_out_queue_create();
	struct VInQueue *in_queue = v_in_queue_create();
	struct Generic_Cmd *_node_destroy, *node_destroy = NULL;
	uint32 node_id;
	uint16 count, len, buffer_len, buffer_pos = 0;
	int8 share;
	int i;
	char buffer[65535] = {0,};
	uint8 last_cmd_id = -1;

	/* Push commands to the queue */
	for(i=0; i<CHUNK_NUM; i++) {
		node_id = 10000+i;

		node_destroy = v_node_destroy_create(node_id);
		v_out_queue_push_tail(out_queue, VRS_DEFAULT_PRIORITY, node_destroy);
	}

	/* Pop commands from the queue */
	for(i=0; i<CHUNK_NUM; i++) {
		count = 0;
		share = 0;
		len = 65535;

		_node_destroy = v_out_queue_pop(out_queue, VRS_DEFAULT_PRIORITY, &count, &share, &len);

		if(_node_destroy->id != last_cmd_id) {
			if(count == 0) {
				len = v_cmd_size(_node_destroy);
			}
			buffer_pos += v_cmd_pack(&buffer[buffer_pos], _node_destroy, len, share);
		} else {
			buffer_pos += v_cmd_pack(&buffer[buffer_pos], _node_destroy, 0, share);
		}

		last_cmd_id = _node_destroy->id;

		v_cmd_destroy(&_node_destroy);
	}

	fail_unless( buffer_pos > 0,
			"Buffer size is zero");

	/* Buffer is sent and received at this point */
	buffer_len = buffer_pos;
	buffer_pos = 0;

	/* Unpack commands from the buffer and push them to the queue of incoming
	 * commands*/
	buffer_pos += v_cmd_unpack(&buffer[buffer_pos], buffer_len, in_queue);

	fail_unless( buffer_pos == buffer_len,
			"Unpacked buffer size: %d != packed buffer size: %d", buffer_pos, buffer_len);

	for(i=0; i<CHUNK_NUM; i++) {
		/* Pop commands from the queue of incoming commands */
		node_id = 10000+i;

		_node_destroy = v_in_queue_pop(in_queue);

		fail_unless( _node_destroy != NULL,
				"Node_Destroy create failed");
		fail_unless( _node_destroy->id == CMD_NODE_DESTROY,
				"Node_Destroy OpCode: %d != %d", _node_destroy->id, CMD_NODE_DESTROY);

		fail_unless( UINT32(_node_destroy->data[0]) == node_id,
				"Node_Destroy Node_ID: %d != %d", UINT32(_node_destroy->data[0]), node_id);

		v_cmd_destroy(&_node_destroy);

		fail_unless( _node_destroy == NULL,
				"Node_Destroy destroy failed");
	}

	v_in_queue_destroy(&in_queue);
	v_out_queue_destroy(&out_queue);
}
END_TEST

/**
 * \brief This function creates test suite for Node_Destroy command
 */
struct Suite *node_destroy_suite(void)
{
	struct Suite *suite = suite_create("Node_Destroy_Cmd");
	struct TCase *tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_Node_Destroy_create);
	tcase_add_test(tc_core, test_Node_Destroy_in_queue);
	tcase_add_test(tc_core, test_Node_Destroy_out_queue);
	tcase_add_test(tc_core, test_Node_Destroy_pack_unpack);

	suite_add_tcase(suite, tc_core);

	return suite;
}
