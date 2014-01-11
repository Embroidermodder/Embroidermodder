/*! @file helpers-unused.h */
#ifndef HELPERS_UNUSED_H
#define HELPERS_UNUSED_H

/*! \def emb_unused(expr)
 *  Supresses compiler warnings for unused variables.
 *  This should only be used on variables that truely are unused.
 *  <b>It should not be used in place of properly fixing compiler warnings.</b> */
#define emb_unused(expr) do { (void)(expr); } while (0)

#endif /* HELPERS_UNUSED_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
