/* filenames.c - Iterator for a list of filenames
 *
 * Copyright © 2010 Intel Corporation
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
 * along with this program.  If not, see http://www.gnu.org/licenses/ .
 *
 * Author: Carl Worth <cworth@cworth.org>
 */

#include "notmuch-private.h"

struct _notmuch_filenames {
    notmuch_filename_node_t *iterator;
};

/* Create a new notmuch_filename_list_t object, with 'ctx' as its
 * talloc owner.
 *
 * This function can return NULL in case of out-of-memory.
 */
notmuch_filename_list_t *
_notmuch_filename_list_create (const void *ctx)
{
    notmuch_filename_list_t *list;

    list = talloc (ctx, notmuch_filename_list_t);
    if (unlikely (list == NULL))
	return NULL;

    list->head = NULL;
    list->tail = &list->head;

    return list;
}

void
_notmuch_filename_list_add_filename (notmuch_filename_list_t *list,
				     const char *filename)
{
    /* Create and initialize new node. */
    notmuch_filename_node_t *node = talloc (list,
					    notmuch_filename_node_t);

    node->filename = talloc_strdup (node, filename);
    node->next = NULL;

    /* Append the node to the list. */
    *(list->tail) = node;
    list->tail = &node->next;
}

void
_notmuch_filename_list_destroy (notmuch_filename_list_t *list)
{
    talloc_free (list);
}

/* The notmuch_filenames_t is an iterator object for a
 * notmuch_filename_list_t */
notmuch_filenames_t *
_notmuch_filenames_create (const void *ctx,
			   notmuch_filename_list_t *list)
{
    notmuch_filenames_t *filenames;

    filenames = talloc (ctx, notmuch_filenames_t);
    if (unlikely (filenames == NULL))
	return NULL;

    filenames->iterator = list->head;
    (void) talloc_reference (filenames, list);

    return filenames;
}

notmuch_bool_t
notmuch_filenames_valid (notmuch_filenames_t *filenames)
{
    if (filenames == NULL)
	return FALSE;

    return (filenames->iterator != NULL);
}

const char *
notmuch_filenames_get (notmuch_filenames_t *filenames)
{
    if (filenames->iterator == NULL)
	return NULL;

    return filenames->iterator->filename;
}

void
notmuch_filenames_move_to_next (notmuch_filenames_t *filenames)
{
    if (filenames->iterator == NULL)
	return;

    filenames->iterator = filenames->iterator->next;
}

void
notmuch_filenames_destroy (notmuch_filenames_t *filenames)
{
    talloc_free (filenames);
}