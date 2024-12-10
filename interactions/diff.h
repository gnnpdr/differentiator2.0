#ifndef _DIFF_H_
#define _DIFF_H_

#include "..\syn_analysis\syn_analysis.h"

static const double SEC = 2; 

Node* diff_node (Node *const node, Err_param *const error);
Node* copy_node (Node *const node, Err_param *const error);

#define ASSERT  do                  \
                {                   \
                    assert(node);   \
                    assert(error);  \
                } while (0);


#endif //_DIFF_H_