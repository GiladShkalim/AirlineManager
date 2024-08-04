/**************/
/*   list.c   */
/**************/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//////////////////////////////////////////
// Init
// Aim:		create new list
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
//////////////////////////////////////////
BOOL L_init(LIST* pList)
{
	if ( pList == NULL ) 	
		return False;	// no list to initialize

	pList->head.next = NULL;
	pList->head.key = NULL;
	return True;
}

/////////////////////////////////////////////////////////////////
// Insert
// Aim:		add new node
// Input:	pointer to the node BEFORE the place for the new one
//			a value to be stored in the new node
// Output:	pointer to the new node
/////////////////////////////////////////////////////////////////
void L_insert(LIST* list, DATA Value)
{
	NODE* tmp;
	NODE* pNode = &(list->head);

	if (!pNode) 
		return;

	tmp = (NODE*)malloc(sizeof(NODE));	// new node

	if (tmp != NULL)  {
		tmp->key = Value;
		tmp->next = pNode->next;
		pNode->next = tmp;
	}
}

//////////////////////////////////////////////////////////////
// Delete
// Aim:		erase node
// Input:	pointer to the node BEFORE the node to be deleted 
// Output:	TRUE if succeeded
//////////////////////////////////////////////////////////////
BOOL L_delete(NODE* pNode)
{
	NODE* tmp;
	if (!pNode)
		return False;
	tmp = pNode->next;
	if(!tmp)
		return False;
	
	pNode->next = tmp->next;
	if (tmp->key != NULL)
	{
		free(tmp ->key);
	}
	free(tmp);
	return True;
}

/////////////////////////////////////////////////////////
// Find
// Aim:		search for a value
// Input:	pointer to the node to start with 
//			a value to be found
// Output:	pointer to the node containing the Value
/////////////////////////////////////////////////////////
const NODE* L_find(const NODE* pNode, DATA Value)
{
	const NODE* tmp = pNode;

	while (tmp != NULL)
	{
		if (tmp->key == Value)
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}

////////////////////////////////////////////////
// Free (additional function)
// Aim:		free the list memory
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
////////////////////////////////////////////////
BOOL L_free(LIST* pList)
{
	NODE *tmp;
	BOOL cont = True;
	if ( !pList ) 
		return False;

	tmp = &(pList->head);
	while (cont)
		cont = L_delete(tmp);
		
	return True;
}

int L_count(LIST* pList)
{
	NODE* tmp;
	int	c = 0;

	if (!pList)
		return 0;

	tmp = pList->head.next;
	while (tmp != NULL)
	{
		c++;
		tmp = tmp->next;
	}
	return c;
}
