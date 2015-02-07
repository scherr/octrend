/* octree.c - octree data structure with functions
 *
 * Maximilian Scherr
 * 2006
 */

#ifndef OCTREE_H
#include "octree.h"
#endif

/* Not documented yet.
 *
 */
char neighbourLookupTable[8][3][3][3][5] =
{
    {
        {
            {
                {1, -1, -1, -1, 7}, // Neighbour of position 0 at (-1, -1, -1).
                {1, -1, -1,  0, 6}, // Neighbour of position 0 at (-1, -1,  0).
                {1, -1, -1,  0, 7}  // Neighbour of position 0 at (-1, -1,  1).
            },
            {
                {1, -1,  0, -1, 5}, // Neighbour of position 0 at (-1,  0, -1).
                {1, -1,  0,  0, 4}, // Neighbour of position 0 at (-1,  0,  0). Important!
                {1, -1,  0,  0, 5}  // Neighbour of position 0 at (-1,  0,  1).
            },
            {
                {1, -1,  0, -1, 7}, // Neighbour of position 0 at (-1,  1, -1).
                {1, -1,  0,  0, 6}, // Neighbour of position 0 at (-1,  1,  0).
                {1, -1,  0,  0, 7}  // Neighbour of position 0 at (-1,  1,  1).
            }
        },
        {
            {
                {1,  0, -1, -1, 3}, // Neighbour of position 0 at ( 0, -1, -1).
                {1,  0, -1,  0, 2}, // Neighbour of position 0 at ( 0, -1,  0). Important!
                {1,  0, -1,  0, 3}  // Neighbour of position 0 at ( 0, -1,  1).
            },
            {
                {1,  0,  0, -1, 1}, // Neighbour of position 0 at ( 0,  0, -1). Important!
                {0,  0,  0,  0, 0}, // Neighbour of position 0 at ( 0,  0,  0).
                {0,  0,  0,  0, 1}  // Neighbour of position 0 at ( 0,  0,  1). Important!
            },
            {
                {1,  0,  0, -1, 3}, // Neighbour of position 0 at ( 0,  1, -1).
                {0,  0,  0,  0, 2}, // Neighbour of position 0 at ( 0,  1,  0). Important!
                {0,  0,  0,  0, 3}  // Neighbour of position 0 at ( 0,  1,  1).
            }
        },
        {
            {
                {1,  0, -1, -1, 7}, // Neighbour of position 0 at ( 1, -1, -1).
                {1,  0, -1,  0, 6}, // Neighbour of position 0 at ( 1, -1,  0).
                {1,  0, -1,  0, 7}  // Neighbour of position 0 at ( 1, -1,  1).
            },
            {
                {1,  0,  0, -1, 5}, // Neighbour of position 0 at ( 1,  0, -1).
                {0,  0,  0,  0, 4}, // Neighbour of position 0 at ( 1,  0,  0). Important!
                {0,  0,  0,  0, 5}  // Neighbour of position 0 at ( 1,  0,  1).
            },
            {
                {1,  0,  0, -1, 7}, // Neighbour of position 0 at ( 1,  1, -1).
                {0,  0,  0,  0, 6}, // Neighbour of position 0 at ( 1,  1,  0).
                {0,  0,  0,  0, 7}  // Neighbour of position 0 at ( 1,  1,  1).
            }
        }
    },
    {
        {
            {
                {1, -1, -1,  0, 6}, // Neighbour of position 1 at (-1, -1, -1).
                {1, -1, -1,  0, 7}, // Neighbour of position 1 at (-1, -1,  0).
                {1, -1, -1,  1, 6}  // Neighbour of position 1 at (-1, -1,  1).
            },
            {
                {1, -1,  0,  0, 4}, // Neighbour of position 1 at (-1,  0, -1).
                {1, -1,  0,  0, 5}, // Neighbour of position 1 at (-1,  0,  0). Important!
                {1, -1,  0,  1, 4}  // Neighbour of position 1 at (-1,  0,  1).
            },
            {
                {1, -1,  0,  0, 6}, // Neighbour of position 1 at (-1,  1, -1).
                {1, -1,  0,  0, 7}, // Neighbour of position 1 at (-1,  1,  0).
                {1, -1,  0,  1, 6}  // Neighbour of position 1 at (-1,  1,  1).
            }
        },
        {
            {
                {1,  0, -1,  0, 2}, // Neighbour of position 1 at ( 0, -1, -1).
                {1,  0, -1,  0, 3}, // Neighbour of position 1 at ( 0, -1,  0). Important!
                {1,  0, -1,  1, 2}  // Neighbour of position 1 at ( 0, -1,  1).
            },
            {
                {0,  0,  0,  0, 0}, // Neighbour of position 1 at ( 0,  0, -1). Important!
                {0,  0,  0,  0, 1}, // Neighbour of position 1 at ( 0,  0,  0).
                {1,  0,  0,  1, 0}  // Neighbour of position 1 at ( 0,  0,  1). Important!
            },
            {
                {0,  0,  0,  0, 2}, // Neighbour of position 1 at ( 0,  1, -1).
                {0,  0,  0,  0, 3}, // Neighbour of position 1 at ( 0,  1,  0). Important!
                {1,  0,  0,  1, 2}  // Neighbour of position 1 at ( 0,  1,  1).
            }
        },
        {
            {
                {1,  0, -1,  0, 6}, // Neighbour of position 1 at ( 1, -1, -1).
                {1,  0, -1,  0, 7}, // Neighbour of position 1 at ( 1, -1,  0).
                {1,  0, -1,  1, 6}  // Neighbour of position 1 at ( 1, -1,  1).
            },
            {
                {0,  0,  0,  0, 4}, // Neighbour of position 1 at ( 1,  0, -1).
                {0,  0,  0,  0, 5}, // Neighbour of position 1 at ( 1,  0,  0). Important!
                {1,  0,  0,  1, 4}  // Neighbour of position 1 at ( 1,  0,  1).
            },
            {
                {0,  0,  0,  0, 6}, // Neighbour of position 1 at ( 1,  1, -1).
                {0,  0,  0,  0, 7}, // Neighbour of position 1 at ( 1,  1,  0).
                {1,  0,  0,  1, 6}  // Neighbour of position 1 at ( 1,  1,  1).
            }
        }
    },
    {
        {
            {
                {1, -1,  0, -1, 5}, // Neighbour of position 2 at (-1, -1, -1).
                {1, -1,  0,  0, 4}, // Neighbour of position 2 at (-1, -1,  0).
                {1, -1,  0,  0, 5}  // Neighbour of position 2 at (-1, -1,  1).
            },
            {
                {1, -1,  0, -1, 7}, // Neighbour of position 2 at (-1,  0, -1).
                {1, -1,  0,  0, 6}, // Neighbour of position 2 at (-1,  0,  0). Important!
                {1, -1,  0,  0, 7}  // Neighbour of position 2 at (-1,  0,  1).
            },
            {
                {1, -1,  1, -1, 5}, // Neighbour of position 2 at (-1,  1, -1).
                {1, -1,  1,  0, 4}, // Neighbour of position 2 at (-1,  1,  0).
                {1, -1,  1,  0, 5}  // Neighbour of position 2 at (-1,  1,  1).
            }
        },
        {
            {
                {1,  0,  0, -1, 1}, // Neighbour of position 2 at ( 0, -1, -1).
                {0,  0,  0,  0, 0}, // Neighbour of position 2 at ( 0, -1,  0). Important!
                {0,  0,  0,  0, 1}  // Neighbour of position 2 at ( 0, -1,  1).
            },
            {
                {1,  0,  0, -1, 3}, // Neighbour of position 2 at ( 0,  0, -1). Important!
                {0,  0,  0,  0, 2}, // Neighbour of position 2 at ( 0,  0,  0).
                {0,  0,  0,  0, 3}  // Neighbour of position 2 at ( 0,  0,  1). Important!
            },
            {
                {1,  0,  1, -1, 1}, // Neighbour of position 2 at ( 0,  1, -1).
                {1,  0,  1,  0, 0}, // Neighbour of position 2 at ( 0,  1,  0). Important!
                {1,  0,  1,  0, 1}  // Neighbour of position 2 at ( 0,  1,  1).
            }
        },
        {
            {
                {1,  0,  0, -1, 5}, // Neighbour of position 2 at ( 1, -1, -1).
                {0,  0,  0,  0, 4}, // Neighbour of position 2 at ( 1, -1,  0).
                {0,  0,  0,  0, 5}  // Neighbour of position 2 at ( 1, -1,  1).
            },
            {
                {1,  0,  0, -1, 7}, // Neighbour of position 2 at ( 1,  0, -1).
                {0,  0,  0,  0, 6}, // Neighbour of position 2 at ( 1,  0,  0). Important!
                {0,  0,  0,  0, 7}  // Neighbour of position 2 at ( 1,  0,  1).
            },
            {
                {1,  0,  1, -1, 5}, // Neighbour of position 2 at ( 1,  1, -1).
                {1,  0,  1,  0, 4}, // Neighbour of position 2 at ( 1,  1,  0).
                {1,  0,  1,  0, 5}  // Neighbour of position 2 at ( 1,  1,  1).
            }
        }
    },
    {
        {
            {
                {1, -1,  0,  0, 4}, // Neighbour of position 3 at (-1, -1, -1).
                {1, -1,  0,  0, 5}, // Neighbour of position 3 at (-1, -1,  0).
                {1, -1,  0,  1, 4}  // Neighbour of position 3 at (-1, -1,  1).
            },
            {
                {1, -1,  0,  0, 6}, // Neighbour of position 3 at (-1,  0, -1).
                {1, -1,  0,  0, 7}, // Neighbour of position 3 at (-1,  0,  0). Important!
                {1, -1,  0,  1, 6}  // Neighbour of position 3 at (-1,  0,  1).
            },
            {
                {1, -1,  1,  0, 4}, // Neighbour of position 3 at (-1,  1, -1).
                {1, -1,  1,  0, 5}, // Neighbour of position 3 at (-1,  1,  0).
                {1, -1,  1,  1, 4}  // Neighbour of position 3 at (-1,  1,  1).
            }
        },
        {
            {
                {0,  0,  0,  0, 0}, // Neighbour of position 3 at ( 0, -1, -1).
                {0,  0,  0,  0, 1}, // Neighbour of position 3 at ( 0, -1,  0). Important!
                {1,  0,  0,  1, 0}  // Neighbour of position 3 at ( 0, -1,  1).
            },
            {
                {0,  0,  0,  0, 2}, // Neighbour of position 3 at ( 0,  0, -1). Important!
                {0,  0,  0,  0, 3}, // Neighbour of position 3 at ( 0,  0,  0).
                {1,  0,  0,  1, 2}  // Neighbour of position 3 at ( 0,  0,  1). Important!
            },
            {
                {1,  0,  1,  0, 0}, // Neighbour of position 3 at ( 0,  1, -1).
                {1,  0,  1,  0, 1}, // Neighbour of position 3 at ( 0,  1,  0). Important!
                {1,  0,  1,  1, 0}  // Neighbour of position 3 at ( 0,  1,  1).
            }
        },
        {
            {
                {0,  0,  0,  0, 4}, // Neighbour of position 3 at ( 1, -1, -1).
                {0,  0,  0,  0, 5}, // Neighbour of position 3 at ( 1, -1,  0).
                {1,  0,  0,  1, 4}  // Neighbour of position 3 at ( 1, -1,  1).
            },
            {
                {0,  0,  0,  0, 6}, // Neighbour of position 3 at ( 1,  0, -1).
                {0,  0,  0,  0, 7}, // Neighbour of position 3 at ( 1,  0,  0). Important!
                {1,  0,  0,  1, 6}  // Neighbour of position 3 at ( 1,  0,  1).
            },
            {
                {1,  0,  1,  0, 4}, // Neighbour of position 3 at ( 1,  1, -1).
                {1,  0,  1,  0, 5}, // Neighbour of position 3 at ( 1,  1,  0).
                {1,  0,  1,  1, 4}  // Neighbour of position 3 at ( 1,  1,  1).
            }
        }
    },
    {
        {
            {
                {1,  0, -1, -1, 3}, // Neighbour of position 4 at (-1, -1, -1).
                {1,  0, -1,  0, 2}, // Neighbour of position 4 at (-1, -1,  0).
                {1,  0, -1,  0, 3}  // Neighbour of position 4 at (-1, -1,  1).
            },
            {
                {1,  0,  0, -1, 1}, // Neighbour of position 4 at (-1,  0, -1).
                {0,  0,  0,  0, 0}, // Neighbour of position 4 at (-1,  0,  0). Important!
                {0,  0,  0,  0, 1}  // Neighbour of position 4 at (-1,  0,  1).
            },
            {
                {1,  0,  0, -1, 3}, // Neighbour of position 4 at (-1,  1, -1).
                {0,  0,  0,  0, 2}, // Neighbour of position 4 at (-1,  1,  0).
                {0,  0,  0,  0, 3}  // Neighbour of position 4 at (-1,  1,  1).
            }
        },
        {
            {
                {1,  0, -1, -1, 7}, // Neighbour of position 4 at ( 0, -1, -1).
                {1,  0, -1,  0, 6}, // Neighbour of position 4 at ( 0, -1,  0). Important!
                {1,  0, -1,  0, 7}  // Neighbour of position 4 at ( 0, -1,  1).
            },
            {
                {1,  0,  0, -1, 5}, // Neighbour of position 4 at ( 0,  0, -1). Important!
                {0,  0,  0,  0, 4}, // Neighbour of position 4 at ( 0,  0,  0).
                {0,  0,  0,  0, 5}  // Neighbour of position 4 at ( 0,  0,  1). Important!
            },
            {
                {1,  0,  0, -1, 7}, // Neighbour of position 4 at ( 0,  1, -1).
                {0,  0,  0,  0, 6}, // Neighbour of position 4 at ( 0,  1,  0). Important!
                {0,  0,  0,  0, 7}  // Neighbour of position 4 at ( 0,  1,  1).
            }
        },
        {
            {
                {1,  1, -1, -1, 3}, // Neighbour of position 4 at ( 1, -1, -1).
                {1,  1, -1,  0, 2}, // Neighbour of position 4 at ( 1, -1,  0).
                {1,  1, -1,  0, 3}  // Neighbour of position 4 at ( 1, -1,  1).
            },
            {
                {1,  1,  0, -1, 1}, // Neighbour of position 4 at ( 1,  0, -1).
                {1,  1,  0,  0, 0}, // Neighbour of position 4 at ( 1,  0,  0). Important!
                {1,  1,  0,  0, 1}  // Neighbour of position 4 at ( 1,  0,  1).
            },
            {
                {1,  1,  0, -1, 3}, // Neighbour of position 4 at ( 1,  1, -1).
                {1,  1,  0,  0, 2}, // Neighbour of position 4 at ( 1,  1,  0).
                {1,  1,  0,  0, 3}  // Neighbour of position 4 at ( 1,  1,  1).
            }
        }
    },
    {
        {
            {
                {1,  0, -1,  0, 2}, // Neighbour of position 5 at (-1, -1, -1).
                {1,  0, -1,  0, 3}, // Neighbour of position 5 at (-1, -1,  0).
                {1,  0, -1,  1, 2}  // Neighbour of position 5 at (-1, -1,  1).
            },
            {
                {0,  0,  0,  0, 0}, // Neighbour of position 5 at (-1,  0, -1).
                {0,  0,  0,  0, 1}, // Neighbour of position 5 at (-1,  0,  0). Important!
                {1,  0,  0,  1, 0}  // Neighbour of position 5 at (-1,  0,  1).
            },
            {
                {0,  0,  0,  0, 2}, // Neighbour of position 5 at (-1,  1, -1).
                {0,  0,  0,  0, 3}, // Neighbour of position 5 at (-1,  1,  0).
                {1,  0,  0,  1, 2}  // Neighbour of position 5 at (-1,  1,  1).
            }
        },
        {
            {
                {1,  0, -1,  0, 6}, // Neighbour of position 5 at ( 0, -1, -1).
                {1,  0, -1,  0, 7}, // Neighbour of position 5 at ( 0, -1,  0). Important!
                {1,  0, -1,  1, 6}  // Neighbour of position 5 at ( 0, -1,  1).
            },
            {
                {0,  0,  0,  0, 4}, // Neighbour of position 5 at ( 0,  0, -1). Important!
                {0,  0,  0,  0, 5}, // Neighbour of position 5 at ( 0,  0,  0).
                {1,  0,  0,  1, 4}  // Neighbour of position 5 at ( 0,  0,  1). Important!
            },
            {
                {0,  0,  0,  0, 6}, // Neighbour of position 5 at ( 0,  1, -1).
                {0,  0,  0,  0, 7}, // Neighbour of position 5 at ( 0,  1,  0). Important!
                {1,  0,  0,  1, 6}  // Neighbour of position 5 at ( 0,  1,  1).
            }
        },
        {
            {
                {1,  1, -1,  0, 2}, // Neighbour of position 5 at ( 1, -1, -1).
                {1,  1, -1,  0, 3}, // Neighbour of position 5 at ( 1, -1,  0).
                {1,  1, -1,  1, 2}  // Neighbour of position 5 at ( 1, -1,  1).
            },
            {
                {1,  1,  0,  0, 0}, // Neighbour of position 5 at ( 1,  0, -1).
                {1,  1,  0,  0, 1}, // Neighbour of position 5 at ( 1,  0,  0). Important!
                {1,  1,  0,  1, 0}  // Neighbour of position 5 at ( 1,  0,  1).
            },
            {
                {1,  1,  0,  0, 2}, // Neighbour of position 5 at ( 1,  1, -1).
                {1,  1,  0,  0, 3}, // Neighbour of position 5 at ( 1,  1,  0).
                {1,  1,  0,  1, 2}  // Neighbour of position 5 at ( 1,  1,  1).
            }
        }
    },
    {
        {
            {
                {1,  0,  0, -1, 1}, // Neighbour of position 6 at (-1, -1, -1).
                {0,  0,  0,  0, 0}, // Neighbour of position 6 at (-1, -1,  0).
                {0,  0,  0,  0, 1}  // Neighbour of position 6 at (-1, -1,  1).
            },
            {
                {1,  0,  0, -1, 3}, // Neighbour of position 6 at (-1,  0, -1).
                {0,  0,  0,  0, 2}, // Neighbour of position 6 at (-1,  0,  0). Important!
                {0,  0,  0,  0, 3}  // Neighbour of position 6 at (-1,  0,  1).
            },
            {
                {1,  0,  1, -1, 1}, // Neighbour of position 6 at (-1,  1, -1).
                {1,  0,  1,  0, 0}, // Neighbour of position 6 at (-1,  1,  0).
                {1,  0,  1,  0, 1}  // Neighbour of position 6 at (-1,  1,  1).
            }
        },
        {
            {
                {1,  0,  0, -1, 5}, // Neighbour of position 6 at ( 0, -1, -1).
                {0,  0,  0,  0, 4}, // Neighbour of position 6 at ( 0, -1,  0). Important!
                {0,  0,  0,  0, 5}  // Neighbour of position 6 at ( 0, -1,  1).
            },
            {
                {1,  0,  0, -1, 7}, // Neighbour of position 6 at ( 0,  0, -1). Important!
                {0,  0,  0,  0, 6}, // Neighbour of position 6 at ( 0,  0,  0).
                {0,  0,  0,  0, 7}  // Neighbour of position 6 at ( 0,  0,  1). Important!
            },
            {
                {1,  0,  1, -1, 5}, // Neighbour of position 6 at ( 0,  1, -1).
                {1,  0,  1,  0, 4}, // Neighbour of position 6 at ( 0,  1,  0). Important!
                {1,  0,  1,  0, 5}  // Neighbour of position 6 at ( 0,  1,  1).
            }
        },
        {
            {
                {1,  1,  0, -1, 1}, // Neighbour of position 6 at ( 1, -1, -1).
                {1,  1,  0,  0, 0}, // Neighbour of position 6 at ( 1, -1,  0).
                {1,  1,  0,  0, 1}  // Neighbour of position 6 at ( 1, -1,  1).
            },
            {
                {1,  1,  0, -1, 3}, // Neighbour of position 6 at ( 1,  0, -1).
                {1,  1,  0,  0, 2}, // Neighbour of position 6 at ( 1,  0,  0). Important!
                {1,  1,  0,  0, 3}  // Neighbour of position 6 at ( 1,  0,  1).
            },
            {
                {1,  1,  1, -1, 1}, // Neighbour of position 6 at ( 1,  1, -1).
                {1,  1,  1,  0, 0}, // Neighbour of position 6 at ( 1,  1,  0).
                {1,  1,  1,  0, 1}  // Neighbour of position 6 at ( 1,  1,  1).
            }
        }
    },
    {
        {
            {
                {0,  0,  0,  0, 0}, // Neighbour of position 7 at (-1, -1, -1).
                {0,  0,  0,  0, 1}, // Neighbour of position 7 at (-1, -1,  0).
                {1,  0,  0,  1, 0}  // Neighbour of position 7 at (-1, -1,  1).
            },
            {
                {0,  0,  0,  0, 2}, // Neighbour of position 7 at (-1,  0, -1).
                {0,  0,  0,  0, 3}, // Neighbour of position 7 at (-1,  0,  0). Important!
                {1,  0,  0,  1, 2}  // Neighbour of position 7 at (-1,  0,  1).
            },
            {
                {1,  0,  1,  0, 0}, // Neighbour of position 7 at (-1,  1, -1).
                {1,  0,  1,  0, 1}, // Neighbour of position 7 at (-1,  1,  0).
                {1,  0,  1,  1, 0}  // Neighbour of position 7 at (-1,  1,  1).
            }
        },
        {
            {
                {0,  0,  0,  0, 4}, // Neighbour of position 7 at ( 0, -1, -1).
                {0,  0,  0,  0, 5}, // Neighbour of position 7 at ( 0, -1,  0). Important!
                {1,  0,  0,  1, 4}  // Neighbour of position 7 at ( 0, -1,  1).
            },
            {
                {0,  0,  0,  0, 6}, // Neighbour of position 7 at ( 0,  0, -1). Important!
                {0,  0,  0,  0, 7}, // Neighbour of position 7 at ( 0,  0,  0).
                {1,  0,  0,  1, 6}  // Neighbour of position 7 at ( 0,  0,  1). Important!
            },
            {
                {1,  0,  1,  0, 4}, // Neighbour of position 7 at ( 0,  1, -1).
                {1,  0,  1,  0, 5}, // Neighbour of position 7 at ( 0,  1,  0). Important!
                {1,  0,  1,  1, 4}  // Neighbour of position 7 at ( 0,  1,  1).
            }
        },
        {
            {
                {1,  1,  0,  0, 0}, // Neighbour of position 7 at ( 1, -1, -1).
                {1,  1,  0,  0, 1}, // Neighbour of position 7 at ( 1, -1,  0).
                {1,  1,  0,  1, 0}  // Neighbour of position 7 at ( 1, -1,  1).
            },
            {
                {1,  1,  0,  0, 2}, // Neighbour of position 7 at ( 1,  0, -1).
                {1,  1,  0,  0, 3}, // Neighbour of position 7 at ( 1,  0,  0). Important!
                {1,  1,  0,  1, 2}  // Neighbour of position 7 at ( 1,  0,  1).
            },
            {
                {1,  1,  1,  0, 0}, // Neighbour of position 7 at ( 1,  1, -1).
                {1,  1,  1,  0, 1}, // Neighbour of position 7 at ( 1,  1,  0).
                {1,  1,  1,  1, 0}  // Neighbour of position 7 at ( 1,  1,  1).
            }
        }
    }
};

/* createOctree:
 * -------------
 *
 *   This function creates a new octree node, that's it.
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...the created octree node or NULL in case memory allocation failed.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     level:    node level,
 *     value:    untyped pointer to node value,
 *     terminal: 0 if node is to be terminal or not 0 if not.
 *
 */
octree *createOctree(unsigned char level, void *value, unsigned char terminal) {
    // Variable declarations.
    octree *node;
    unsigned char i;

    // Memory allocation for octree node.
    node = malloc(sizeof(octree));
    // Error handling.
    if (node == NULL) {
        printf("createOctree <NULL>: Not enough memory available!\n");
        return NULL;
    }

    // Initialization of structure elements.
    node->level = level;
    node->position = 0;
    node->parent = NULL;
    node->childCount = 0;
    for (i = 0; i < 8; i++) {
        node->children[i] = NULL;
    }
    node->value = value;
    node->terminal = terminal;

    return node;
}

/* cloneOctree:
 * -------------
 *
 *   This function clones an existing octree node.
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...the cloned octree node or NULL if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:       the source node,
 *     cloneValue: pointer to a function that is supposed to clone an octree value
 *                 (if NULL cloned value becomes NULL!).
 *
 */
octree *cloneOctree(octree *node, void *(*cloneValue)(void *)) {
    // Variable declarations.
    octree *cloneNode;
    unsigned char i;

    // Error handling.
    if (node == NULL) {
        printf("cloneOctree <NULL>: Argument \"node\" is NULL!\n");
        return NULL;
    }

    // Clone node creation.
    cloneNode = createOctree(node->level, NULL, node->terminal);
    // Error handling.
    if (cloneNode == NULL) {
        printf("cloneOctree <NULL>: External error occured in \"createOctree\".\n");
        return NULL;
    }

    // Copying childCount value.
    node->childCount = cloneNode->childCount;

    // Cloning of node children.
    for (i = 0; i < 8; i++) {
        if (node->children[i] == NULL) {
            cloneNode->children[i] = NULL;
        } else {
            cloneNode->children[i] = cloneOctree(node->children[i], cloneValue);
            cloneNode->children[i]->parent = cloneNode;
        }
    }

    // Cloning of node value.
    if (cloneValue != NULL) {
        cloneNode->value = (*cloneValue)(node->value);
    }

    return cloneNode;
}

/* deleteOctree:
 * -------------
 *
 *   This function deletes an octree node and all its descendant nodes.
 *   Be careful with deleting the top level node!
 *
 *   Returns...
 *   ----------
 *
 *     ...0 if success or not 0 if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:        the node to be deleted,
 *     deleteValue: pointer to a function that is supposed to delete an octree value
 *                  (if NULL and octree value not NULL, octree value memory is freed).
 *
 */
int deleteOctree(octree *node, int (*deleteValue)(void*)) {
    // Variable declarations.
    unsigned char i;
    int result;

    // Error handling.
    if (node == NULL) {
        printf("deleteOctree <1>: Argument \"node\" is NULL!\n");
        return 1;
    }

    // Removing of node from its parent's children list.
    if (node->parent != NULL) {
        node->parent->children[node->position] = NULL;
        node->parent->childCount = node->parent->childCount - 1;
    }

    // Deletion of descendant nodes.
    for (i = 0; i < 8; i++) {
        if (node->children[i] != NULL) {
            deleteOctree(node->children[i], deleteValue);
        }
    }

    // Deletion of octree value.
    if (node->value != NULL) {
        if (deleteValue != NULL) {
            result = (*deleteValue)(node->value);
            // Error handling.
            if (result != 0) {
                printf("deleteOctree <%d>: External error occured in \"deleteValue\".\n", 100 + result);
                return 100 + result;
            }
        } else {
            // Freeing of octree value memory!
            free(node->value);
        }
    }

    // Freeing of node memory!
    free(node);

    return 0;
}

/* removeOctree:
 * -------------
 *
 *   This function removes an octree node, which is not only deleting a node but also deleting
 *   redundant nodes! Here's a binary tree example:
 *
 *    [ ] node to be deleted,
 *    *   terminal node,
 *    O   non terminal node.
 *
 *         _O_                     _O_ <-----+
 *        /   \    deleteOctree   /   \      |
 *       O     O   ============> O     O     |
 *      / \   / \                 \   / \    |
 *    [*]  * *   *                 * *   *   |
 *                                           +- no difference here
 *         _O_                     _O_       |
 *        /   \    removeOctree   /   \      |
 *       O     O   ============> O     O     |
 *      / \   / \                 \   / \    |
 *    [*]  * *   *                 * *   * <-+
 *
 *         _O_                     _O_ <-----+
 *        /   \    deleteOctree   /   \      |
 *       O     O   ============> O     O     |
 *        \   / \                     / \    |
 *        [*]*   *                   *   *   |
 *                                           +- difference: deleteOctree doesn't delete the node(s)
 *         _O_                      O_       |              that don't have any terminal descendants,
 *        /   \    removeOctree       \      |              removeOctree does.
 *       O     O   ============>       O     |
 *        \   / \                     / \    |
 *        [*]*   *                   *   * <-+
 *
 *
 *   Be careful with deleting the top level node!
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...0 if success or not 0 if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:        the node to be removed,
 *     deleteValue: pointer to a function that is supposed to delete an octree value
 *                  (if NULL and octree value not NULL, octree value memory is freed).
 *
 */
int removeOctree(octree *node, int (*deleteValue)(void*)) {
    // Variable declarations.
    int result;

    // Error handling.
    if (node == NULL) {
        printf("removeOctree <1>: Argument \"node\" is NULL!\n");
        return 1;
    }

    if (node->parent != NULL) {
        if (node->parent->childCount == 1 && node->parent->terminal == 0 && node->parent->parent != NULL) {
            // If parent has no child after deletion of this node travel upwards.
            removeOctree(node->parent, deleteValue);
        } else {
            // We don't have to travel upwards if the conditions above aren't met.
            deleteOctree(node, deleteValue);
        }
    } else {
        // Node is top level node, so removing is the same as deleting.
        deleteOctree(node, deleteValue);
    }

    return 0;
}

/* getTopLevelNode:
 * ----------------
 *
 *   This function finds the top level node of an octree node.
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...the top level node or NULL if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:        the node.
 *
 */
octree *getTopLevelNode(octree *node) {
    if (node == NULL) {
        printf("getTopLevelNode <NULL>: Argument \"node\" is NULL!\n");
        return NULL;
    }
    if (node->parent == NULL) {
        return node;
    } else {
        return getTopLevelNode(node->parent);
    }
}

/* insertOctree:
 * -------------
 *
 *   This function inserts an octree node into another octree node. Here's a binary tree examples:
 *
 *    { } node,
 *    ( ) position,
 *    [ ] node to be inserted.
 *
 *         _{A}_ ----+
 *        /     \    |
 *       B      (C)  |                      _A_
 *      / \     / \  |                     /   \
 *     D   E   F   G +- insertOctree =>   B     H
 *                   |                   / \   / \
 *         [H]       |                  D   E I   J
 *         / \       |
 *        I   J -----+
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...0 if success or not 0 if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:             the node where to insert,
 *     nodeToBeInserted: the node to be inserted (node and nodeToBeInserted may not already reside
 *                       in the same tree),
 *     position:         the position in node where nodeToBeInserted should be inserted,
 *     deleteValue:      pointer to a function that is supposed to delete an octree value
 *                       (if NULL and octree value not NULL, octree value memory is freed).
 *
 */
int insertOctree(octree *node, octree *nodeToBeInserted, unsigned char position, int (*deleteValue)(void *)) {
    // Variable declarations.
    int result;

    // Error handling.
    if (node == NULL) {
        printf("insertOctree <1>: Argument \"node\" is NULL!\n");
        return 1;
    }
    if (nodeToBeInserted == NULL) {
        printf("insertOctree <2>: Argument \"nodeToBeInserted\" is NULL!\n");
        return 2;
    }
    if (getTopLevelNode(node) == getTopLevelNode(nodeToBeInserted)) {
        printf("insertOctree <3>: \"node\" and \"nodeToBeInserted\" may not already reside in the same tree!\n");
        return 3;
    }
    if (node->level == 0) {
        printf("insertOctree <4>: No more insertions possible at level 0.\n");
        return 4;
    }
    if (node->level - 1 != nodeToBeInserted->level) {
        printf("insertOctree <5>: Incompatible levels.\n");
        return 5;
    }
    if (position > 7) {
        printf("insertOctree <6>: Invalid position (%d). Expected range is from 0 to 7.\n", position);
        return 6;
    }

    // Structural updating of node to be inserted.
    nodeToBeInserted->position = position;
    nodeToBeInserted->parent = node;

    // Structural updating of node.
    node->terminal = 0;
    if (node->children[position] != NULL) {
        result = deleteOctree(node->children[position], deleteValue);
        if (result != 0) {
            printf("insertOctree <%d>: External error occured in \"deleteOctree\".\n", 100 + result);
            return 100 + result;
        }
    }
    node->childCount = node->childCount + 1;
    node->children[position] = nodeToBeInserted;

    return 0;
}

/* addOctree:
 * -------------
 *
 *   This function adds a new terminal child node to an octree node. Deletion (because of replacement)
 *   can be involved. Here are some binary tree examples:
 *
 *    { } node,
 *    ( ) position,
 *    *   terminal node,
 *    O   non terminal node.
 *
 *         _O_                     _O_
 *        /   \     addOctree     /   \
 *       O    {O}   =========>   O     O
 *      / \   /                 / \   / \
 *     *   * *  ( )            *   * *   *
 *
 *         _{O}_                   _O_
 *        /     \   addOctree     /   \
 *       O      (O) =========>   O     *
 *      / \     /               / \
 *     *   *   *               *   *
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...0 if success or not 0 if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:        the node where to add a new node,
 *     position:    the position in node where a new node should be added,
 *     value:       untyped pointer to node value, which becomes node value of added node,
 *     deleteValue: pointer to a function that is supposed to delete an octree value
 *                  (if NULL and octree value not NULL, octree value memory is freed).
 *
 */
int addOctree(octree *node, unsigned char position, void *value, int (*deleteValue)(void *)) {
    // Variable declarations.
    int result;

    // Error handling.
    if (node == NULL) {
        printf("addOctree <1>: Argument \"node\" is NULL!\n");
        return 1;
    }
    if (position > 7) {
        printf("addOctree <2>: Invalid position (%d). Expected range is from 0 to 7.\n", position);
        return 2;
    }

    // Insertion of new node with nested creation of new node.
    result = insertOctree(node, createOctree(node->level - 1, value, 1), position, deleteValue);
    // Error handling.
    if (result != 0) {
        printf("addOctree <%d>: External error occured in \"insertOctree\".\n", 100 + result);
        return 100 + result;
    }

    return 0;
}

/* expandOctree:
 * -------------
 *
 *   This function expands an octree node, which means that the node's children list or those of
 *   their ancestors, depending on amount of levels to expand, is filled with new terminal nodes.
 *   However existing children nodes are not manipulated!
 *   Here are some binary tree examples:
 *
 *    { } node,
 *    *   terminal node,
 *    O   non terminal node.
 *
 *         _O_                                  _O_
 *        /   \    expandOctree (1 level)      /   \
 *       O    {O}  =======================>   O     O
 *      / \   /                              / \   / \
 *     *   * *                              *   * *   *
 *
 *         _{O}_                                _O_
 *        /     \  expandOctree (1 level)      /   \
 *       O       O =======================>   O     O
 *      / \     /                            / \   /
 *     *   *   *                            *   * *
 *
 *          {O}                                 _O_
 *                 expandOctree (1 level)      /   \
 *                 =======================>   *     *
 *
 *
 *
 *          {O}                                 _O_
 *                 expandOctree (2 levels)     /   \
 *                 =======================>   *     *
 *                                           / \   / \
 *                                          *   * *   *
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...0 if success or not 0 if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:       the node to expand,
 *     levels:     the amount of levels to expand,
 *     cloneValue: pointer to a function that is supposed to clone an octree value
 *                 (if NULL cloned value becomes NULL!).
 *
 */
int expandOctree(octree *node, unsigned char levels, void *(*cloneValue)(void *)) {
    // Variable declarations.
    unsigned char i;
    void *value;
    int result;

    // Error handling.
    if (node == NULL) {
        printf("expandOctree <1>: Argument \"node\" is NULL!\n");
        return 1;
    }

    if (levels > node->level) {
        levels = node->level;
    }
    // 0 levels means nothing to do.
    if (levels == 0) {
        return 0;
    }

    for (i = 0; i < 8; i++) {
        // Only add nodes at this level where there's no child yet.
        if (node->children[i] == NULL) {
            // Value cloning.
            if (node->value == NULL) {
                value = NULL;
            } else {
                if (cloneValue == NULL) {
                    value = NULL;
                } else {
                    value = (*cloneValue)(node->children[i]->value);
                }
            }

            // Add child node.
            result = addOctree(node, i, value, NULL);
            // Error handling.
            if (result != 0) {
                printf("expandOctree <%d>: External error occured in \"addOctree\".\n", 100 + result);
                return 100 + result;
            }
        }

        // If more than this level has to be expanded the child nodes are also expanded...
        if (levels > 1) {
            result = expandOctree(node->children[i], levels - 1, cloneValue);
            // Error handling.
            if (result != 0) {
                printf("expandOctree <%d>: External error occured in \"expandOctree\".\n", 100 + result);
                return 100 + result;
            }
        }
    }

    return 0;
}

/* updateOctreeDownward:
 * ---------------------
 *
 *   This function updates the node values of an octree (although orientation is downward) in a
 *   bottom-up way, which means that a root node has to be selected and a level to which (from the
 *   bottom of that tree) values should be updated. Updating is done by joining all the children node
 *   values to one node value. Here are some binary tree examples with numbers as values and a
 *   joinValues function, that calculates average values (no child => value = 0):
 *
 *    { } node,
 *    *   terminal node,
 *    O   non terminal node.
 *
 *         _1_                                         _1_
 *        /   \    updateOctreeDownward (level 1)     /   \
 *      {2}    3   ==============================>  4.5    3
 *      / \   /                                     / \   /
 *     4   5 6                                     4   5 6
 *
 *         _{1}_                                       _1_
 *        /     \  updateOctreeDownward (level 1)     /   \
 *       2       3 ==============================>  4.5    3
 *      / \     /                                   / \   /
 *     4   5   6                                   4   5 6
 *
 *         _{1}_                                       _3.75_
 *        /     \  updateOctreeDownward (level 2)     /      \
 *       2       3 ==============================>  4.5       3
 *      / \     /                                   / \      /
 *     4   5   6                                   4   5    6
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...0 if success or not 0 if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:       the root node which itself and its descendants should be updated,
 *     level:      the highest level to update (not relative to the root node level!),
 *     joinValues: pointer to a function that is supposed to join 8 values into one
 *                 (mustn't be NULL!).
 *
 */
int updateOctreeDownward(octree *node, unsigned char level, int (*joinValues)(void *[], void **)) {
    // Variable Declarations.
    int result;
    unsigned char i;
    void *values[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

    // Error handling.
    if (node == NULL) {
        printf("updateOctreeDownward <1>: Argument \"node\" is NULL!\n");
        return 1;
    }
    if (joinValues == NULL) {
        printf("updateOctreeDownward <2>: Argument \"joinValues\" is NULL!\n");
        return 2;
    }

    if (node->terminal != 0) {
        // Nothing to do if node is terminal.
        return 0;
    }

    if (node->level > level) {
        // Calling updateOctreeDownward for children, but doing nothing else.
        for (i = 0; i < 8; i++) {
            if (node->children[i] != NULL && node->children[i]->terminal == 0) {
                result = updateOctreeDownward(node->children[i], level, joinValues);
                // Error handling.
                if (result != 0) {
                    printf("updateOctreeDownward <%d>: External error occured in \"updateOctreeDownward\".\n", 100 + result);
                    return 100 + result;
                }
            }
        }
    } else {
        if (node->level > 1) {
            // Calling updateOctreeDownward for children.
            for (i = 0; i < 8; i++) {
                if (node->children[i] != NULL && node->children[i]->terminal == 0) {
                    result = updateOctreeDownward(node->children[i], level, joinValues);
                    // Error handling.
                    if (result != 0) {
                        printf("updateOctreeDownward <%d>: External error occured in \"updateOctreeDownward\".\n", 100 + result);
                        return 100 + result;
                    }
                }
            }
        }

        // Collecting node values of children.
        for (i = 0; i < 8; i++) {
            if (node->children[i] != NULL) {
                values[i] = node->children[i]->value;
            }
        }
        // Joining collected node values to this node's value.
        result = (*joinValues)(values, &node->value);
        if (result != 0) {
            // Error handling.
            printf("updateOctreeDownward <%d>: External error occured in \"joinValues\".\n", 100 + result);
            return 100 + result;
        }
    }

    return 0;
}

/* updateOctreeUpward:
 * -------------------
 *
 *   This function updates the node values of all the parents of a given node up to a given level.
 *   It's quite useful for updating after changing a terminal node.
 *
 *    { } node,
 *    *   terminal node,
 *    O   non terminal node.
 *
 *         _1_                                     _2.5_
 *        /   \  updateOctreeUpward (level 2)     /     \
 *      {2}    3 ============================>   2       3
 *      / \   /                                 / \     /
 *     4   5 6                                 4   5   6
 *
 *         _1_                                    _1_
 *        /   \  updateOctreeUpward (level 1)    /   \
 *       2     3 ============================> 4.5    3
 *      / \   /                                / \   /
 *     4  {5}6                                4   5 6
 *
 *         _1_                                    _3.75_
 *        /   \  updateOctreeUpward (level 2)    /      \
 *       2     3 ============================> 4.5       3
 *      / \   /                                / \      /
 *     4  {5}6                                4   5    6
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...0 if success or not 0 if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:       the root node whose ancestors should be updated,
 *     level:      the highest level to update (not relative to the root node level!),
 *     joinValues: pointer to a function that is supposed to join 8 values into one
 *                 (mustn't be NULL!).
 *
 */
int updateOctreeUpward(octree *node, unsigned char level, int (*joinValues)(void *[], void **)) {
    // Variable Declarations.
    int result;
    unsigned char i;
    void *values[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

    // Error handling.
    if (node == NULL) {
        printf("updateOctreeUpward <1>: Argument \"node\" is NULL!\n");
        return 1;
    }
    if (joinValues == NULL) {
        printf("updateOctreeUpward <2>: Argument \"joinValues\" is NULL!\n");
        return 2;
    }

    if (node->level >= level) {
        // Nothing to do if node's level is above level or on same level that level indicates.
        return 0;
    }

    if (node->parent != NULL) {
        // Collecting node values of nodes on same level as this one.
        for (i = 0; i < 8; i++) {
            if (node->parent->children[i] != NULL) {
                values[i] = node->parent->children[i]->value;
            }
        }
        // Joining collected node values to this node's parent's value.
        result = (*joinValues)(values, &node->parent->value);
        if (result != 0) {
            // Error handling.
            printf("updateOctreeUpward <%d>: External error occured in \"joinValues\".\n", 100 + result);
            return 100 + result;
        }

        if (node->level + 1 < level && node->parent->parent != NULL) {
            // Calling updateOctreeUpward for parent.
            updateOctreeUpward(node->parent, level, joinValues);
        }
    }

    return 0;
}

/* collapseOctree:
 * ---------------
 *
 *   This function does the opposite of what expandOctree does. Children values are not joined but
 *   updateOctreeDownward can be called before collapseOctree to provide that feature.
 *   Here are some binary tree examples:
 *
 *    { } node,
 *    *   terminal node,
 *    O   non terminal node.
 *
 *         _O_                           _O_
 *        /   \      collapseOctree     /   \
 *       O    {O}    ==============>   O     *
 *      / \   /                       / \
 *     *   * *                       *   *
 *
 *         _O_                           _*_
 *        /   \      collapseOctree     /   \
 *       O    {O}    ==============>   O     *
 *      / \   / \                     / \
 *     *   * *   *                   *   *
 *
 *         _{O}_                          *
 *        /     \    collapseOctree
 *       O       O   ==============>
 *      / \     / \
 *     *   *   *   *
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...0 if success or not 0 if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:        the node to collapse,
 *     deleteValue: pointer to a function that is supposed to delete an octree value
 *                  (if NULL and octree value not NULL, octree value memory is freed).
 *
 */
int collapseOctree(octree *node, int (*deleteValue)(void*)) {
    // Value declarations.
    unsigned char i;
    int result;

    // Error handling.
    if (node == NULL) {
        printf("collapseOctree <1>: Argument \"node\" is NULL!\n");
        return 1;
    }

    if (node->terminal != 0) {
        // Nothing to do if node is terminal.
        return 0;
    }

    // The node becomes terminal.
    node->terminal = 1;

    for (i = 0; i < 8; i++) {
        // Deleting children.
        if (node->children[i] != NULL) {
            result = deleteOctree(node->children[i], deleteValue);
            // Error handling.
            if (result != 0) {
                printf("collapseOctree <%d>: External error occured in \"deleteOctree\".\n", 100 + result);
                return 100 + result;
            }
        }
    }

    return 0;
}

/* compressOctreeDownward:
 * ----------------------
 *
 *   This function does something similar to what collapseOctree does. The difference is that that it's done
 *   for a node and for all descendants of a node and only those nodes are collapsed which have 8 children with
 *   equal values and if those nodes are at or are below a given level. Calling updateOctreeDownward before calling
 *   compressOctree grants "good compression" in case (upward) updates aren't performed after each change on
 *   terminal node. Here are some binary tree examples:
 *
 *    { } node,
 *
 *         _{1}_                                _1_
 *        /     \  compressOctree (level 2)    /   \
 *       2       3 ========================>  2     3
 *      / \     /                                  /
 *     2   2   3                                  3
 *
 *         _1_                                   _1_
 *        /   \    compressOctree (level 2)     /   \
 *      {2}    3   ========================>   2     3
 *      / \   /                               / \   /
 *     2   4 3                               2   4 3
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...0 if success or not 0 if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:        the root node of the tree to compress,
 *     equalValues: pointer to a function that is supposed to compare two octree values for equality
 *                  (if NULL and all children octree values NULL, compression is done),
 *     deleteValue: pointer to a function that is supposed to delete an octree value
 *                  (if NULL and octree value not NULL, octree value memory is freed).
 *
 */
int compressOctreeDownward(octree *node, unsigned char level, int (*equalValues)(void*, void*), int (*deleteValue)(void*)) {
    // Variable declarations.
    unsigned char i;
    unsigned char count;
    int result;

    // Error handling.
    if (node == NULL) {
        printf("compressOctreeDownward <1>: Argument \"node\" is NULL!\n");
        return 1;
    }

    if (node->terminal != 0) {
        // Nothing to do if node is terminal.
        return 0;
    }

    // Counting children and calling compressOctreeDownward for each of them if they are not terminal.
    count = 0;
    for (i = 0; i < 8; i++) {
        if (node->children[i] != NULL) {
            ++count;
            if (node->children[i]->terminal == 0) {
                result = compressOctreeDownward(node->children[i], level, equalValues, deleteValue);
                if (result != 0) {
                    printf("compressOctreeDownward <%d>: External error occured in \"compressOctreeDownward\".\n", 100 + result);
                    return 100 + result;
                }
            }
        }
    }

    if (node->level <= level && count == 8) {
        // If node is at or below given level and if node has 8 children check for equality.
        for (i = 0; i < 7; i++) {
            if (equalValues == NULL) {
                if (node->children[i]->value != NULL || node->children[i + 1]->value != NULL) {
                    break;
                }
            } else {
                if(!(*equalValues)(node->children[i]->value, node->children[i + 1]->value)) {
                    break;
                }
            }
        }
        if (i == 7) {
            // If they are equal collapse node.
            result = collapseOctree(node, deleteValue);
            // Error handling.
            if (result != 0) {
                printf("compressOctreeDownward <%d>: External error occured in \"collapseOctree\".\n", 100 + result);
                return 100 + result;
            }
        }
    }

    return 0;
}

/* Not documented yet.
 *
 */
int compressOctreeUpward(octree *node, unsigned char level, int (*equalValues)(void*, void*), int (*deleteValue)(void*)) {
    // Not commented yet.
    unsigned char i;
    int result;
    octree *parent;
    if (node == NULL) {
        printf("compressOctreeUpward <1>: Argument \"node\" is NULL!\n");
        return 1;
    }
    if (node->level >= level) {
        return 0;
    }
    if (node->parent != NULL) {
        parent = node->parent;
        for (i = 0; i < 8; i++) {
            if (parent->children[i] != NULL) {
                break;
            }
        }
        if (i == 8) {
            for (i = 0; i < 7; i++) {
                if (equalValues == NULL) {
                    if (parent->children[i]->value != NULL || parent->children[i + 1]->value != NULL) {
                        break;
                    }
                } else {
                    if(!(*equalValues)(parent->children[i]->value, parent->children[i + 1]->value)) {
                        break;
                    }
                }
            }
            if (i == 7) {
                result = collapseOctree(parent, deleteValue);
                if (result != 0) {
                    printf("compressOctreeUpward <%d>: External error occured in \"collapseOctree\".\n", 100 + result);
                    return 100 + result;
                }
                if (node->level + 1 < level && parent->parent != NULL) {
                    compressOctreeUpward(parent, level, equalValues, deleteValue);
                }
            }
        }
    }
    return 0;
}

/* getOctreeCoordinates:
 * ---------------------
 *
 *   This function calculates the coordinates of a node (relative to a top level node and its
 *   coordinates). The space and cube orientation is best described by ASCII diagrams:
 *
 *    z    x    +-+-+         +-+-+
 *    *   *   +-+-+7|  +-+-+  |5|7|
 *    *  *    |1|3|-+  |1|3|  +-+-+
 *    * *     +-+-+6|  +-+-+  |4|6|
 *    **      |0|2|-+  |0|2|  +-+-+
 *    o****y  +-+-+    +-+-+
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...coordinates if success or {0, 0, 0} if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     node:           the node we are trying to get coordinates from,
 *     topLevel:       the reference node's level,
 *     topCoordinates: the reference node's coordinates,
 *
 */
intVector getOctreeCoordinates(octree *node, unsigned char topLevel, intVector topCoordinates){
    // Not commented yet.
    intVector coordinates = {0, 0, 0};
    int singleOffset;
    intVector offsetVector = {0, 0, 0};
    intVector parentCoordinates;
    if (node == NULL) {
        printf("getOctreeCoordinates <{0, 0, 0}>: Argument \"node\" is NULL!\n");
        return coordinates;
    }
    if (node->level >= topLevel) {
        coordinates.x = topCoordinates.x;
        coordinates.y = topCoordinates.y;
        coordinates.z = topCoordinates.z;
    } else {
        singleOffset = 1 << node->level;
        switch (node->position) {
            case 0:
                break;
            case 1:
                offsetVector.z = singleOffset;
                break;
            case 2:
                offsetVector.y = singleOffset;
                break;
            case 3:
                offsetVector.y = singleOffset;
                offsetVector.z = singleOffset;
                break;
            case 4:
                offsetVector.x = singleOffset;
                break;
            case 5:
                offsetVector.x = singleOffset;
                offsetVector.z = singleOffset;
                break;
            case 6:
                offsetVector.x = singleOffset;
                offsetVector.y = singleOffset;
                break;
            case 7:
                offsetVector.x = singleOffset;
                offsetVector.y = singleOffset;
                offsetVector.z = singleOffset;
                break;
            default:
                printf("getOctreeCoordinates <{0, 0, 0}>: Invalid position (%d). Expected range is from 0 to 7.\n", node->position);
                return coordinates;
        }
        if (node->parent != NULL) {
            parentCoordinates = getOctreeCoordinates(node->parent, topLevel, topCoordinates);
            coordinates = addIntVector(parentCoordinates, offsetVector);
        }
    }
    return coordinates;
}

/* findTerminalOctree:
 * ---------------------
 *
 *   This function finds the terminal node (relative to a top level node and its coordinates) which
 *   occupies space at given coordinates. Read more in the section about the arguments.
 *
 *
 *   Returns...
 *   ----------
 *
 *     ...the node if success or NULL if an error occured.
 *
 *
 *   Arguments:
 *   ----------
 *
 *     coordinates:    the coordinates where to search,
 *     terminalLevel:  the level where every non terminal node with children is regarded as terminal,
 *     topNode:        the reference node's level,
 *     topCoordinates: the reference node's coordinates,
 *
 */
octree *findTerminalOctree(intVector coordinates, unsigned char terminalLevel, octree *topNode, intVector topCoordinates) {
    // Not commented yet.
    int limit;
    int limitHalf;
    int position;
    intVector newTopCoords = {0, 0, 0};
    if (topNode == NULL) {
        printf("findTerminalOctree <NULL>: Argument \"topNode\" is NULL!\n");
        return NULL;
    }
    if (topNode->terminal != 0 || topNode->level <= terminalLevel) {
        return topNode;
    }
    coordinates = subIntVector(coordinates, topCoordinates);
    limit = 1 << topNode->level;
    limitHalf = limit >> 1;
    position = 0;
    if (coordinates.x >= limitHalf && coordinates.x < limit) {
        position = 4;
        newTopCoords.x = newTopCoords.x + limitHalf;
    }
    if (coordinates.y >= limitHalf && coordinates.y < limit) {
        position = position + 2;
        newTopCoords.y = newTopCoords.y + limitHalf;
    }
    if (coordinates.z >= limitHalf && coordinates.z < limit) {
        position = position + 1;
        newTopCoords.z = newTopCoords.z + limitHalf;
    }
    if (topNode->children[position] == NULL) {
        return NULL;
    } else {
        return findTerminalOctree(coordinates, terminalLevel, topNode->children[position], newTopCoords);
    }
}

/* Not documented yet.
 *
 */
octree *addTerminalOctree(intVector coordinates, unsigned char level, void *value, octree *topNode, intVector topCoordinates,
        int (*deleteValue)(void *),	void *(*cloneValue)(void *)) {
    // Not commented yet.
    int limit;
    int limitHalf;
    int position;
    void *clonedVal;
    intVector newTopCoords = {0, 0, 0};
    if (topNode == NULL) {
        printf("addTerminalOctree <NULL>: Argument \"topNode\" is NULL!\n");
        return NULL;
    }
    if (topNode->level <= level) {
        if (topNode->terminal == 0) {
            collapseOctree(topNode, deleteValue);
        }
        if (topNode->value != NULL) {
            (*deleteValue)(topNode->value);
        }
        topNode->value = value;
        return topNode;
    }
    coordinates = subIntVector(coordinates, topCoordinates);
    limit = 1 << topNode->level;
    limitHalf = limit >> 1;
    position = 0;
    if (coordinates.x >= limitHalf && coordinates.x < limit) {
        position = 4;
        newTopCoords.x = newTopCoords.x + limitHalf;
    }
    if (coordinates.y >= limitHalf && coordinates.y < limit) {
        position = position + 2;
        newTopCoords.y = newTopCoords.y + limitHalf;
    }
    if (coordinates.z >= limitHalf && coordinates.z < limit) {
        position = position + 1;
        newTopCoords.z = newTopCoords.z + limitHalf;
    }

    if (topNode->children[position] == NULL) {
        if (value == NULL) {
            clonedVal = NULL;
        } else {
            if (cloneValue == NULL) {
                clonedVal = NULL;
            } else {
                clonedVal = (*cloneValue)(value);
            }
        }
        addOctree(topNode, position, clonedVal, deleteValue);
    } else {
        if (topNode->children[position]->terminal != 0 && topNode->level - 1 > level) {
            expandOctree(topNode->children[position], 1, cloneValue);
        }
    }
    return addTerminalOctree(coordinates, level, value, topNode->children[position], newTopCoords, deleteValue, cloneValue);
}

/* Not documented yet.
 *
 */
intVector getVirtualOctreeCoordinates(octree node, unsigned char topLevel, intVector topCoordinates) {
    // Not commented yet.
    intVector coordinates = {0, 0, 0};
    int singleOffset;
    intVector offsetVector = {0, 0, 0};
    intVector parentCoordinates;
    if (node.level >= topLevel) {
        coordinates.x = topCoordinates.x;
        coordinates.y = topCoordinates.y;
        coordinates.z = topCoordinates.z;
    } else {
        singleOffset = 1 << node.level;
        switch (node.position) {
            case 0:
                break;
            case 1:
                offsetVector.z = singleOffset;
                break;
            case 2:
                offsetVector.y = singleOffset;
                break;
            case 3:
                offsetVector.y = singleOffset;
                offsetVector.z = singleOffset;
                break;
            case 4:
                offsetVector.x = singleOffset;
                break;
            case 5:
                offsetVector.x = singleOffset;
                offsetVector.z = singleOffset;
                break;
            case 6:
                offsetVector.x = singleOffset;
                offsetVector.y = singleOffset;
                break;
            case 7:
                offsetVector.x = singleOffset;
                offsetVector.y = singleOffset;
                offsetVector.z = singleOffset;
                break;
            default:
                printf("getOctreeCoordinates <{0, 0, 0}>: Invalid position (%d). Expected range is from 0 to 7.\n", node.position);
                return coordinates;
        }
        if (node.parent != NULL) {
            parentCoordinates = getOctreeCoordinates(node.parent, topLevel, topCoordinates);
            coordinates = addIntVector(parentCoordinates, offsetVector);
        }
    }
    return coordinates;
}

/* Not documented yet.
 *
 */
octree findVirtualOctree(intVector coordinates, unsigned char terminalLevel, octree *topNode, intVector topCoordinates) {
    // Not commented yet.
    int limit;
    int limitHalf;
    int position;
    octree resultVirtualNode;

    intVector newTopCoords = {0, 0, 0};
    if (topNode == NULL) {
        printf("findVirtualOctree <1>: Argument \"topNode\" is NULL!\n");
        return resultVirtualNode;
    }
    if (topNode->terminal != 0) {
        return *topNode;
    }
    if (topNode->level <= terminalLevel) {
        resultVirtualNode = *topNode;
        resultVirtualNode.terminal = 1;
        return resultVirtualNode;
    }

    coordinates = subIntVector(coordinates, topCoordinates);
    limit = 1 << topNode->level;
    limitHalf = limit >> 1;
    position = 0;
    if (coordinates.x >= limitHalf && coordinates.x < limit) {
        position = 4;
        newTopCoords.x = newTopCoords.x + limitHalf;
    }
    if (coordinates.y >= limitHalf && coordinates.y < limit) {
        position = position + 2;
        newTopCoords.y = newTopCoords.y + limitHalf;
    }
    if (coordinates.z >= limitHalf && coordinates.z < limit) {
        position = position + 1;
        newTopCoords.z = newTopCoords.z + limitHalf;
    }
    if (topNode->children[position] == NULL) {
        resultVirtualNode.level = topNode->level - 1;
        resultVirtualNode.parent = topNode;
        resultVirtualNode.position = position;
        resultVirtualNode.childCount = 0;
        resultVirtualNode.terminal = 0;
        return resultVirtualNode;
    } else {
        return findVirtualOctree(coordinates, terminalLevel, topNode->children[position], newTopCoords);
    }
}

/* Not documented yet.
 *
 */
octree findNeighbour(octree currentVirtualNode, intVector direction) {
    // Not commented yet.
    char tableX = (direction.x == 0 ? 1 : (direction.x > 0 ? 2 : 0));
    char tableY = (direction.y == 0 ? 1 : (direction.y > 0 ? 2 : 0));
    char tableZ = (direction.z == 0 ? 1 : (direction.z > 0 ? 2 : 0));
    char tableLookup[5] =
    {
        neighbourLookupTable[currentVirtualNode.position][tableX][tableY][tableZ][0],
        neighbourLookupTable[currentVirtualNode.position][tableX][tableY][tableZ][1],
        neighbourLookupTable[currentVirtualNode.position][tableX][tableY][tableZ][2],
        neighbourLookupTable[currentVirtualNode.position][tableX][tableY][tableZ][3],
        neighbourLookupTable[currentVirtualNode.position][tableX][tableY][tableZ][4]
    };
    octree resultVirtualNode;
    octree *tempNode;
    octree parentVirtualNeighbour;
    octree *parentNeighbour;
    intVector parentNeighbourDirection = {0, 0, 0};

    if (currentVirtualNode.parent == NULL) {
        resultVirtualNode.level = currentVirtualNode.level;
        resultVirtualNode.parent = NULL;
        resultVirtualNode.position = 0;
        resultVirtualNode.childCount = 0;
        resultVirtualNode.terminal = 0;
        return resultVirtualNode;
    }

    if (tableLookup[0] == 0) {
        // In same parent node.
        tempNode = currentVirtualNode.parent->children[tableLookup[4]];
        if (tempNode != NULL) {
            resultVirtualNode = *tempNode;
            return resultVirtualNode;
        } else {
            resultVirtualNode.level = currentVirtualNode.level;
            resultVirtualNode.parent = currentVirtualNode.parent;
            resultVirtualNode.position = tableLookup[4];
            resultVirtualNode.childCount = 0;
            resultVirtualNode.terminal = 0;
            return resultVirtualNode;
        }
    } else {
        // In different parent node.
        parentNeighbourDirection.x = tableLookup[1];
        parentNeighbourDirection.y = tableLookup[2];
        parentNeighbourDirection.z = tableLookup[3];
        parentVirtualNeighbour = findNeighbour(*currentVirtualNode.parent, parentNeighbourDirection);

        if (parentVirtualNeighbour.parent == NULL) {
            resultVirtualNode = parentVirtualNeighbour;
            return resultVirtualNode;
        }

        // Here we try to get the parent's real neighbour.
        parentNeighbour = parentVirtualNeighbour.parent->children[parentVirtualNeighbour.position];
        if (parentNeighbour == NULL) {
            resultVirtualNode = parentVirtualNeighbour;
            return resultVirtualNode;
        } else {
            tempNode = parentNeighbour->children[tableLookup[4]];
            if (tempNode != NULL) {
                resultVirtualNode = *tempNode;
                return resultVirtualNode;
            } else {
                resultVirtualNode.level = parentNeighbour->level - 1;
                resultVirtualNode.parent = parentNeighbour;
                resultVirtualNode.position = tableLookup[4];
                resultVirtualNode.childCount = 0;
                resultVirtualNode.terminal = 0;
                return resultVirtualNode;
            }
        }
    }
}

/* Not documented yet.
 *
 */
int isOctreeDescendantOf(octree *maybeDescendant, octree *node) {
    // Not commented yet.
    char levelDiff = node->level - maybeDescendant->level;
    unsigned char i;

    if (levelDiff < 0) {
        return 0;
    }

    if (levelDiff == 0) {
        return maybeDescendant == node;
    }
    return isOctreeDescendantOf(maybeDescendant->parent, node);
}

/* Not documented yet.
 *
 */
int isOctreeAncestorOf(octree *maybeAncestor, octree *node) {
    // Not commented yet.
    return isOctreeDescendantOf(node, maybeAncestor);
}

/* Not documented yet.
 *
 */
int isRecursivelyTerminalOctree(octree *node) {
    // Not commented yet.
    unsigned char i;

    if (node->terminal != 0) {
        return 1;
    }

    if (node->childCount < 8) {
        return 0;
    }

    for (i = 0; i < 8; i++) {
        if (!isRecursivelyTerminalOctree(node->children[i])) {
            return 0;
        }
    }

    return 1;
}

/* Not documented yet.
 *
 */
int isNeighbourOf(octree *maybeNeighbour, octree *node) {
    // Not commented yet.
    intVector directions[26] =
    {
        {-1, -1, -1},
        {-1, -1,  0},
        {-1, -1,  1},
        {-1,  0, -1},
        {-1,  0,  0},
        {-1,  0,  1},
        {-1,  1, -1},
        {-1,  1,  0},
        {-1,  1,  1},
        { 0, -1, -1},
        { 0, -1,  0},
        { 0, -1,  1},
        { 0,  0, -1},
        // { 0,  0,  0},
        { 0,  0,  1},
        { 0,  1, -1},
        { 0,  1,  0},
        { 0,  1,  1},
        { 1, -1, -1},
        { 1, -1,  0},
        { 1, -1,  1},
        { 1,  0, -1},
        { 1,  0,  0},
        { 1,  0,  1},
        { 1,  1, -1},
        { 1,  1,  0},
        { 1,  1,  1}
    };
    unsigned char i;
    octree neighbour;

    if (maybeNeighbour == node) {
        return 1;
    }

    for (i = 0; i < 26; i++) {
        neighbour = findNeighbour(*node, directions[i]);
        if (neighbour.parent == maybeNeighbour->parent && neighbour.position == maybeNeighbour->position) {
            return 1;
        }
    }

    return 0;
}

/* Not documented yet.
 *
 */
int printOctree(octree *node) {
    // Not commented yet.
    unsigned char i;
    if (node == NULL) {
        printf("printOctree <1>: Argument \"node\" is NULL!\n");
        return 1;
    }
    for (i = 0; i < node->level; i++) {
        printf(" ");
    }
    // printf("[%d, %d, %d, %d, %d, %d, %d]\n", node, node->parent, node->position, node->level, node->childCount, node->value, node->terminal);
    printf("[%d, %d, %d, %d, %d, %d]\n", (node->parent == NULL ? -1 : node->parent->position), node->position, node->level, node->childCount, (int) node->value, node->terminal);
    if (node->level > 0) {
        for (i = 0; i < 8; i++) {
            if (node->children[i] != NULL) {
                printOctree(node->children[i]);
            }
        }
    }
    return 0;
}

/* Not documented yet.
 *
 */
unsigned int countOctrees(octree *node) {
    unsigned char i;
    unsigned int acc = 1;
    if (node == NULL) {
        printf("countOctrees <0>: Argument \"node\" is NULL!\n");
        return 0;
    }
    if (node->terminal != 0) {
        return 1;
    }
    for (i = 0; i < 8; i++) {
        if (node->children[i] != NULL) {
            acc = acc + countOctrees(node->children[i]);
        }
    }
    return acc;
}

/* Not documented yet.
 *
 */
unsigned int countTerminalOctrees(octree *node) {
    unsigned char i;
    unsigned int acc = 0;
    if (node == NULL) {
        printf("countTerminalOctrees <0>: Argument \"node\" is NULL!\n");
        return 0;
    }
    if (node->terminal != 0) {
        return 1;
    }
    for (i = 0; i < 8; i++) {
        if (node->children[i] != NULL) {
            acc = acc + countTerminalOctrees(node->children[i]);
        }
    }
    return acc;
}

/* Not documented yet.
 *
 */
int drawOctreeZPlane(octree *node, int z, unsigned char terminalLevel) {
    // Not commented yet.
    int i;
    int x;
    int y;
    intVector topCoordinates = {0,0,0};
    intVector coordinates = {0, 0, z};
    octree *tempNode;
    if (node == NULL) {
        printf("printOctree <1>: Argument \"node\" is NULL!\n");
        return 1;
    }
    printf("Z = %d, terminal level = %d.\n\n", z, terminalLevel);
    printf("+");
    for (i = 0; i < 1 << node->level; i++) {
        printf("-");
    }
    printf("+\n");
    for (x = (1 << node->level) - 1; x >= 0; x--) {
        printf("|");
        for (y = 0; y < 1 << node->level; y++){
            coordinates.x = x;
            coordinates.y = y;
            tempNode = findTerminalOctree(coordinates, terminalLevel, node, topCoordinates);
            if (tempNode == NULL) {
                printf(" ");
            } else {
                printf("%d", tempNode->level);
                // printf("*");
            }
        }
        printf("|\n");
    }
    printf("+");
    for (i = 0; i < 1 << node->level; i++) {
        printf("-");
    }
    printf("+\n");
    return 0;
}

/* Only for testing purpose.
 *
 */
/*
int main(void) {
    return 0;
}
*/
