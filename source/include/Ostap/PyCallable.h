// ============================================================================
#ifndef OSTAP_PYCALLABLE_H 
#define OSTAP_PYCALLABLE_H 1
// ============================================================================
// Include files
// ============================================================================
// Python
// ============================================================================
struct  _object ;
typedef _object PyObject ;
// ============================================================================
namespace Ostap
{
  // ==========================================================================
  namespace Functions 
  {
    // ========================================================================
    /** @class PyCallable Ostap/PyCallable.h
     *  Simple C++   wrapper for the python callable 
     *  @author Vanya Belyaev
     *  @date   2019-09-25
     */
    class PyCallable final 
    {
    public  : 
      // ======================================================================
      /// constructor from the callable object 
      PyCallable ( PyObject* callable , const bool ok ) ;
      /// copy constructor 
      PyCallable ( const PyCallable&  right ) ;
      /// Move constructor 
      PyCallable (       PyCallable&& right ) ;
      /// Destructor 
      ~PyCallable () ;
      // ======================================================================
    public:
      // ======================================================================
      /// the main method:
      double operator () ( const double x ) const { return evaluate ( x ) ; }
      /// the main method 
      double evaluate    ( const double x ) const ;
      // ======================================================================
    private :
      // ======================================================================
      // the callable object 
      PyObject* m_callable   { nullptr } ;
      // the tuple of arguments 
      PyObject* m_arguments  { nullptr } ;
      // ======================================================================
    };
    // ========================================================================
  } //                                    The end of namespace Ostap::Functions 
  // ==========================================================================
} //                                                 The end of namespace Ostap 
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // OSTAP_PYCALLABLE_H
// ============================================================================
