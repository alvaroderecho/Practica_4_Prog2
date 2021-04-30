#include "vertex.h"
#include <string.h>
#define NO_ID -1

struct _Vertex
{
  long id;
  char tag[TAG_LENGTH];
  Label state;

  int index;
};
/*----------------------------------------------------------------------------------------*/
/*
Private functions:
*/
Status vertex_setField(Vertex *v, char *key, char *value);


Status vertex_setField(Vertex *v, char *key, char *value)
{
  if (!key || !value)
    return ERROR;

  if (strcmp(key, "id") == 0)
  {
    return vertex_setId(v, atol(value));
  }
  else if (strcmp(key, "tag") == 0)
  {
    return vertex_setTag(v, value);
  }
  else if (strcmp(key, "state") == 0)
  {
    return vertex_setState(v, (Label)atoi(value));
  }
  else if (strcmp(key,"index") == 0){
    return vertex_setIndex(v,(int)atoi(value));
  }

  return ERROR;
}

/*----------------------------------------------------------------------------------------*/
Vertex *vertex_initFromString(char *descr)
{
  char buffer[1024];
  char *token;
  char *key;
  char *value;
  char *p;
  Vertex *v;

  /* Check args: */
  if (!descr)
    return NULL;

  /* Allocate memory for vertex: */
  v = vertex_init();
  if (!v)
    return NULL;

  /* Read and tokenize description: */
  sprintf(buffer, "%s", descr);
  token = strtok(buffer, " \t\n");
  while (token)
  {
    p = strchr(token, ':');
    if (!p)
      continue;

    *p = '\0';
    key = token;
    value = p + 1;

    vertex_setField(v, key, value);

    token = strtok(NULL, " \t\n");
  }

  return v;
}
Vertex *vertex_init()
{

  Vertex *v;

  v = (Vertex *)malloc(sizeof(Vertex));

if (v==NULL) return NULL;
  v->id=0;
  strcpy(v->tag," ");
  v->state=WHITE;
  v->index=0;
  return v;
}

void vertex_free(void *v)
{
  free(v);
}

long vertex_getId(const Vertex *v)
{
  if (!v)
    return NO_ID;
  return v->id;
}

const char *vertex_getTag(const Vertex *v)
{
  if (!v)
    return NULL;
  return v->tag;
}

Label vertex_getState(const Vertex *v)
{
  if (!v)
    return ERROR_VERTEX;
  return v->state;
}

Status vertex_setId(Vertex *v, const long id)
{
  if (!v || id < 0)
    return ERROR;
  v->id = id;
  return OK;
}

Status vertex_setTag(Vertex *v, const char *tag)
{
  if (!v || strlen(tag) > TAG_LENGTH)
    return ERROR;
  strcpy(v->tag, tag);

  return OK;
}

Status vertex_setState(Vertex *v, const Label state)
{
  if (!v)
    return ERROR;
  v->state = state;
  return OK;
}
int vertex_getIndex(const Vertex *v){
  if (v == NULL) return -1;
  return v->index;
}
Status vertex_setIndex(Vertex *v, int Index){
  if (v == NULL) return ERROR;
  v->index=Index;
  return OK;
}

int vertex_cmp(const void *v1, const void *v2)
{
  int more=1, less=-1, same=0;

  
  if ((vertex_getId(v1)) > (vertex_getId(v2)))
  {
    
    return more;
  }
  else if ((vertex_getId(v1)) < (vertex_getId(v2)))
  {
    
    return less;
  }
  else if ((vertex_getId(v1)) == (vertex_getId(v2)))
  {
    
    return same;
  }
  else
    return 0;
}
void *vertex_copy(const void *src)
{
  Vertex *trg;
  trg = vertex_init();
  if (trg==NULL) return NULL;
  vertex_setId(trg,vertex_getId(src));
  vertex_setTag(trg,vertex_getTag(src));
  vertex_setState(trg,vertex_getState(src));
  return trg;
}

int vertex_print (FILE * pf, const void * v){
  
  //pf= fopen("stdout","w");
  if (pf==NULL) return -1;
  fprintf (pf,"[%ld, %s, %d, %d]",vertex_getId(v),vertex_getTag(v),vertex_getState(v),vertex_getIndex(v));
  
  return strlen(vertex_getTag(v));
}



/**  rest of the functions in vertex.h **/
