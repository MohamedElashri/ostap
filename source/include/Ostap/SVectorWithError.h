// ============================================================================
#ifndef OSTAP_SVECTORWITHERROR_H 
#define OSTAP_SVECTORWITHERROR_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL 
// ============================================================================
#include <iosfwd>
// ============================================================================
// ROOT
// ============================================================================
#include "Math/SVector.h"
#include "Math/SMatrix.h"
// ============================================================================
// Ostap
// ============================================================================
#include "Ostap/ValueWithError.h"
// ============================================================================
namespace Ostap
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    /** @class SVectorWithError Ostap/SVectorWithError.h
     *  Simple class with represent SVector with 
     *  the associated covariance matrix
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    template <unsigned int N, typename SCALAR=double>
    class SVectorWithError
    {
      // ======================================================================
    public:
      // ======================================================================
      /// the actual type of data
      typedef ROOT::Math::SVector<SCALAR,N>                             Value ;
      /// the actual type of covarinace matrix
      typedef ROOT::Math::SMatrix<SCALAR,N,N,ROOT::Math::MatRepSym<SCALAR,N> > Covariance ;
      /// self type 
      typedef SVectorWithError<N,SCALAR>                                Self  ;
      // ======================================================================
    public:
      // ======================================================================
      enum {
            /// vector size
            kSize = N // vector size 
      } ;  
      // ======================================================================
    public:
      // ======================================================================
      /// full constructor from vector and covariance matrix 
      SVectorWithError 
      ( const Value&      value = Value()       , 
        const Covariance& cov2  = Covariance () ) 
        : m_value ( value ) 
        , m_cov2  ( cov2  ) 
      {}
      /// full constructor from covariance matrix 
      SVectorWithError ( const Covariance& cov2 ) 
        : m_value (       ) 
        , m_cov2  ( cov2  ) 
      {}
      /// construct from expressions
      template <class B, class R>
      SVectorWithError 
      ( const Value&                            value , 
        const ROOT::Math::Expr<B,SCALAR,N,N,R>& cov2  ) 
        : m_value ( value )
        , m_cov2  ( cov2  ) 
      {}
      /// constructor from different scalar types 
      template <class B>
      SVectorWithError 
      ( const ROOT::Math::VecExpr<B,SCALAR,N>& value , 
        const Covariance&                      cov2  ) 
        : m_value ( value )
        , m_cov2  ( cov2  ) 
      {}
      /// construct from expressions
      template <class B1, class B2, class R>
      SVectorWithError 
      ( const ROOT::Math::VecExpr<B1,SCALAR,N>&  value , 
        const ROOT::Math::Expr<B2,SCALAR,N,N,R>& cov2  ) 
        : m_value ( value )
        , m_cov2  ( cov2  ) 
      {}
      /// construct from expressions
      template <class B>
      SVectorWithError 
      ( const ROOT::Math::VecExpr<B,SCALAR,N>& value ) 
        : m_value ( value )
        , m_cov2  (       ) 
      {}
      /// construct from expressions
      template <class B, class R>
      SVectorWithError 
      ( const ROOT::Math::Expr<B,SCALAR,N,N,R>& cov2 ) 
        : m_value (       )
        , m_cov2  ( cov2  ) 
      {}
      // ======================================================================
    public: // trivial accessors 
      // ======================================================================
      const  Value&      value       () const { return m_value      ; }
      const  Covariance& cov2        () const { return m_cov2       ; }
      const  Covariance& covariance  () const { return this->cov2() ; }
      // ======================================================================
      inline Value&      value       ()       { return m_value      ; }
      inline Covariance& cov2        ()       { return m_cov2       ; }      
      inline Covariance& covariance  ()       { return this->cov2() ; }
      // ======================================================================
      const  SCALAR& value ( unsigned int i ) const  { return m_value ( i )     ; }
      const  SCALAR& cov2  ( unsigned int i , 
                             unsigned int j ) const  { return m_cov2  ( i , j ) ; }
      // ======================================================================
      inline SCALAR& value ( unsigned int i )        { return m_value ( i )     ; }
      inline SCALAR& cov2  ( unsigned int i , 
                             unsigned int j )        { return m_cov2  ( i , j ) ; }      
      // ======================================================================
    public:  // finally it is just a vector 
      // ======================================================================
      const  SCALAR& operator() ( unsigned int i ) const { return m_value(i) ; }
      inline SCALAR& operator() ( unsigned int i )       { return m_value(i) ; }
      const  SCALAR& operator[] ( unsigned int i ) const { return m_value[i] ; }
      inline SCALAR& operator[] ( unsigned int i )       { return m_value[i] ; }
      // ======================================================================
    public: // correlations 
      // ======================================================================
      /** get the correlation coefficient between "i" and "j"
       *  for invalid setup , return large negative value 
       *  @param i the first index 
       *  @param j the second index 
       *  @return correlation coefficient 
       */
      inline SCALAR  corr  ( unsigned int i  , unsigned int j  ) const ;
      /** get the full correlation matrix 
       *  @return false for invalid setup 
       */
      inline bool    corr  ( Covariance& corrm ) const ;
      // ======================================================================
    public: // setters 
      // ======================================================================
      void setValue      ( const Value&      v ) { m_value = v ; }
      void setCovariance ( const Covariance& c ) { m_cov2  = c ; }
      void setCov2       ( const Covariance& c ) { m_cov2  = c ; }
      // ======================================================================
      template <class B>
      void setValue       ( const ROOT::Math::VecExpr<B,SCALAR,N>&  v ) 
      { m_value = v ; }
      template <class B, class R>
      void setCovariance  ( const ROOT::Math::Expr<B,SCALAR,N,N,R>& c ) 
      { m_cov2 = c ; }
      template <class B, class R>
      void setCov2        ( const ROOT::Math::Expr<B,SCALAR,N,N,R>& c ) 
      { m_cov2 = c ; }
      // ======================================================================
    public: // cast:
      // ======================================================================
      operator const Value&      () const { return value () ; }
      operator       Value&      ()       { return value () ; }
      operator const Covariance& () const { return cov2  () ; }
      operator       Covariance& ()       { return cov2  () ; }
      // ======================================================================
    public: //  operators
      // ======================================================================
      Self& operator+= ( const Self&   right ) 
      { m_value += right.m_value ; m_cov2 += right.m_cov2 ; return *this ; }
      Self& operator-= ( const Self&   right ) 
      { m_value -= right.m_value ; m_cov2 += right.m_cov2 ; return *this ; }
      Self& operator+= ( const Value&  right ) 
      { m_value += right                                  ; return *this ; }
      Self& operator-= ( const Value&  right ) 
      { m_value -= right                                  ; return *this ; }
      Self& operator*= ( const double  s     ) 
      { m_value *= s             ; m_cov2 *= (s*s)        ; return *this ; }
      Self& operator/= ( const double  s     ) 
      { m_value /= s             ; m_cov2 /= (s*s)        ; return *this ; }
      // ======================================================================
      template <class B>
      Self&  operator+= ( const ROOT::Math::VecExpr<B,SCALAR,N>&  right ) 
      { m_value += right ; return *this ; }
      template <class B>
      Self&  operator-= ( const ROOT::Math::VecExpr<B,SCALAR,N>&  right ) 
      { m_value -= right ; return *this ; }
      // ======================================================================
      // unary- 
      Self operator-() const { return Self ( -m_value , m_cov2 ) ; }  // unary- 
      // ======================================================================
    public: //  chi2 distances
      // ======================================================================
      double chi2 ( const Self&  right ) const ;
      double chi2 ( const Value& right ) const ;      
      template <class B>
      double chi2 ( const ROOT::Math::VecExpr<B,SCALAR,N>&  right ) const ;
      // ======================================================================
    public:  // more functions 
      // ======================================================================
      /// calculate the weighted average for two vectors 
      inline Self mean    ( const Self& right ) const ;
      /// calculate the weighted average for two vectors 
      inline Self average ( const Self& right ) const { return mean ( right ) ; }
      // ======================================================================
    public: //  helper functions for pythonizations
      // ======================================================================
      Self  __add__     ( const Self&  right ) const ;
      Self  __sub__     ( const Self&  right ) const ;      
      Self  __add__     ( const Value& right ) const ;
      Self  __sub__     ( const Value& right ) const ;      
      Self  __radd__    ( const Value& right ) const ;
      Self  __rsub__    ( const Value& right ) const ;
      // ======================================================================
      Self  __mul__     ( const double v     ) const { return (*this) *  v ; }
      Self  __truediv__ ( const double v     ) const { return (*this) /  v ; }
      Self  __div__     ( const double v     ) const { return (*this) /  v ; }
      Self  __rmul__    ( const double v     ) const { return (*this) *  v ; }
      // ======================================================================
      Self& __imul__    ( const double v     )       { return (*this) *= v ; }
      Self& __idiv__    ( const double v     )       { return (*this) /= v ; }
      Self& __iadd__    ( const double v     )       { return (*this) += v ; }
      Self& __isub__    ( const double v     )       { return (*this) -= v ; }
      // ======================================================================
    public: //  printout 
      // ======================================================================
      /// printpout 
      std::ostream& fillStream ( std::ostream& s ) const ;         // printpout 
      /// conversion to string
      std::string   toString   () const ;               // conversion to string
      // ======================================================================
    private:
      // ======================================================================
      /// the data 
      Value      m_value  ;                                         // the data 
      /// the covariance matrix
      Covariance m_cov2   ;                           // the covarinance matrix  
      // ======================================================================
    } ;
    // ========================================================================
    /// disable null-size vectors
    template <class SCALAR>
    class SVectorWithError<0,SCALAR> {} ;
    // ========================================================================
    /** Get element of the vector as ValueWithError Object 
     *  @code
     *  SVectorWithError<5> vct = ... ;
     *  get<1> ( vct ) ;
     *  @endcode 
     *  @see Ostap::Math::ValueWithError 
     */
    template <unsigned int I  ,  
              unsigned int N  , 
              typename SCALAR ,
              typename = std::enable_if<(I<N)> > 
    inline 
    ValueWithError
    get
    ( const SVectorWithError<N,SCALAR>& v ) 
    { return ValueWithError ( v.value ( I ) , v.cov2 ( I , I ) ) ; }
    // ===============================================================
    /** Get element of the vector as ValueWithError Object 
     *  @code
     *  SVectorWithError<5> vct = ... ;
     *  get ( vct , 1 ) ;
     *  @endcode 
     *  @see Ostap::Math::ValueWithError 
     */
     template <unsigned int N  , typename SCALAR >
     inline 
     ValueWithError
     get
     ( const SVectorWithError<N,SCALAR>& v , 
       const unsigned short              i )
     { return i < N ? ValueWithError ( v.value ( i ) , v.cov2 ( i , i ) ) : ValueWithError()  ; }
    // ========================================================================
    /** Get element of the vector as ValueWithError Object 
     *  @code
     *  SVectorWithError<5> vct = ... ;
     *  get ( 1 , vct ) ;
     *  @endcode 
     *  @see Ostap::Math::ValueWithError 
     */
     template <unsigned int N  , typename SCALAR >
     inline 
     ValueWithError
     get
     ( const unsigned short              i , 
       const SVectorWithError<N,SCALAR>& v )
     { return i < N ? ValueWithError ( v.value ( i ) , v.cov2 ( i , i ) ) : ValueWithError()  ; }
    // ========================================================================
    // /// specific case for N=1 
    // template <> 
    // class SVectorWithError<1,double> : public Ostap::Math::ValueWithError
    // {
    // public:
    //   // =======================================================================
    //   /// the actual type of data
    //   typedef ROOT::Math::SVector<double,1>                                    Value       ;
    //   /// the actual type of covarinace matrix
    //   typedef ROOT::Math::SMatrix<double,1,1,ROOT::Math::MatRepSym<double,1> > Covariance ;
    //   // ======================================================================
    //   enum {
    //     /// vector size
    //     kSize = 1 // vector size 
    //   } ;  
    //   // ======================================================================
    // public:
    //   // ======================================================================
    //   SVectorWithError 
    //   ( const double value = 0 , 
    //     const double cov2  = 0 ) 
    //     : ValueWithError ( value , cov2 ) 
    //   {}
    //   // ======================================================================
    //   SVectorWithError
    //   ( const Value&      value , 
    //     const Covariance& cov2  )
    //     : SVectorWithError ( value [ 0 ] , cov2 ( 0 , 0 ) ) 
    //   {}
    //   // ======================================================================
    //   /// constructor from expressions 
    //   template <class B>
    //   SVectorWithError 
    //   ( const ROOT::Math::VecExpr<B,double,1>& value , 
    //     const Covariance&                      cov2  ) 
    //     : SVectorWithError ( Value ( value ) , cov2  ) 
    //   {}
    //   /// constructor from expressions
    //   template <class B, class R>
    //   SVectorWithError 
    //   ( const Value&                            value , 
    //     const ROOT::Math::Expr<B,double,1,1,R>& cov2  ) 
    //     : SVectorWithError ( value , Covariance ( cov2 ) ) 
    //   {}
    //   /// constructor from expressions
    //   template <class B1, class B2, class R>
    //   SVectorWithError 
    //   ( const ROOT::Math::VecExpr<B1,double,1>&  value , 
    //     const ROOT::Math::Expr<B2,double,1,1,R>& cov2  ) 
    //     : SVectorWithError ( Value ( value ) , cov2  ) 
    //   {}
    //   // ======================================================================      
    // } ;  
    // ========================================================================
    /// printout 
    template <unsigned int N, class SCALAR> 
    inline std::ostream& operator<<
    ( std::ostream& s , const SVectorWithError<N,SCALAR>& vct ) 
    { return vct.fillStream ( s ) ; }
    // ========================================================================
    template <unsigned int N, class SCALAR> 
    inline double chi2 
    ( const SVectorWithError<N,SCALAR>&      v1 , 
      const SVectorWithError<N,SCALAR>&      v2 ) { return v1.chi2 ( v2 )  ; }
    // ========================================================================
    template <unsigned int N, class SCALAR> 
    inline double chi2 
    ( const ROOT::Math::SVector<SCALAR,N>&   v1 , 
      const SVectorWithError<N,SCALAR>&      v2 ) { return v2.chi2 ( v1 )  ; }
    // ========================================================================
    template <unsigned int N, class SCALAR> 
    inline double chi2 
    ( const SVectorWithError<N,SCALAR>&       v1 , 
      const ROOT::Math::SVector<SCALAR,N>&    v2 ) { return v1.chi2 ( v2 )  ; }
    // ========================================================================
    template <unsigned int N, class SCALAR, class B> 
    inline double chi2 
    (  const ROOT::Math::VecExpr<B,SCALAR,N>& v1 , 
       const SVectorWithError<N,SCALAR>&      v2 ) { return v2.chi2 ( v1 )  ; }
    // ========================================================================
    template <unsigned int N, class SCALAR, class B> 
    inline double chi2 
    ( const SVectorWithError<N,SCALAR>&       v1 , 
      const ROOT::Math::VecExpr<B,SCALAR,N>&  v2 ) { return v1.chi2 ( v2 )  ; }
    // ========================================================================
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR>
    operator+ 
    ( const SVectorWithError<N,SCALAR>& v1 , 
      const SVectorWithError<N,SCALAR>& v2 ) 
    {
      SVectorWithError<N,SCALAR> tmp ( v1 ) ;
      return tmp += v2 ;
    }
    // ========================================================================
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR>
    operator- 
    ( const SVectorWithError<N,SCALAR>& v1 , 
      const SVectorWithError<N,SCALAR>& v2 ) 
    {
      SVectorWithError<N,SCALAR> tmp ( v1 ) ;
      return tmp -= v2 ;
    }
    // ========================================================================    
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR>
    operator+ 
    ( const SVectorWithError<N,SCALAR>&    v1 , 
      const ROOT::Math::SVector<SCALAR,N>& v2 ) 
    {
      SVectorWithError<N,SCALAR> tmp ( v1 ) ;
      return tmp += v2 ;
    }
    // ========================================================================
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR>
    operator+ 
    ( const ROOT::Math::SVector<SCALAR,N>& v2 ,
      const SVectorWithError<N,SCALAR>&    v1 ) { return v1 + v2 ; }
    // ========================================================================
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR>
    operator- 
    ( const SVectorWithError<N,SCALAR>&    v1 , 
      const ROOT::Math::SVector<SCALAR,N>& v2 )  
    {
      SVectorWithError<N,SCALAR> tmp ( v1 ) ;
      return tmp -= v2 ;
    }
    // ========================================================================
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR>
    operator- 
    ( const ROOT::Math::SVector<SCALAR,N>& v2 ,
      const SVectorWithError<N,SCALAR>&    v1 ) 
    {
      return SVectorWithError<N,SCALAR> ( v2 - v1.value() , v1.cov2()  ) ;
    }
    // ========================================================================
    template <unsigned int N, class SCALAR, class B>
    inline 
    SVectorWithError<N,SCALAR>
    operator+ 
    ( const SVectorWithError<N,SCALAR>&      v1 , 
      const ROOT::Math::VecExpr<B,SCALAR,N>& v2 ) 
    {
      SVectorWithError<N,SCALAR> tmp ( v1 ) ;
      return tmp += v2 ;
    }
    // ========================================================================
    template <unsigned int N, class SCALAR, class B>
    inline 
    SVectorWithError<N,SCALAR>
    operator- 
    ( const SVectorWithError<N,SCALAR>&      v1 , 
      const ROOT::Math::VecExpr<B,SCALAR,N>& v2 ) 
    {
      SVectorWithError<N,SCALAR> tmp ( v1 ) ;
      return tmp -= v2 ;
    }
    // ========================================================================
    template <unsigned int N, class SCALAR, class B>
    inline 
    SVectorWithError<N,SCALAR>
    operator+ 
    ( const ROOT::Math::VecExpr<B,SCALAR,N>& v2 , 
      const SVectorWithError<N,SCALAR>&      v1 ) { return v1 + v2 ; }
    // ========================================================================
    template <unsigned int N, class SCALAR, class B>
    inline 
    SVectorWithError<N,SCALAR>
    operator- 
    ( const ROOT::Math::VecExpr<B,SCALAR,N>& v2 , 
      const SVectorWithError<N,SCALAR>&      v1 ) 
    {
      return SVectorWithError<N,SCALAR> ( v2 - v1.value() , v1.cov2()  ) ;
    }
    // ========================================================================
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR>
    operator* 
    ( const SVectorWithError<N,SCALAR>&      v1 , 
      const SCALAR                           v2 ) 
    {
      SVectorWithError<N,SCALAR> tmp ( v1 ) ;
      return tmp *= v2 ;
    }
    // ========================================================================
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR>
    operator/
    ( const SVectorWithError<N,SCALAR>&      v1 , 
      const SCALAR                           v2 ) 
    {
      SVectorWithError<N,SCALAR> tmp ( v1 ) ;
      return tmp /= v2 ;
    }
    // ========================================================================
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR>
    operator* 
    ( const SCALAR                           v2 , 
      const SVectorWithError<N,SCALAR>&      v1 )
    {
      SVectorWithError<N,SCALAR> tmp ( v1 ) ;
      return tmp *= v2 ;
    }
    // ========================================================================
    /// evaluate the mean of a and b 
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR> mean 
    ( const SVectorWithError<N,SCALAR>& v1 , 
      const SVectorWithError<N,SCALAR>& v2 ) { return v1.mean ( v2 ) ; }
    /// evaluate the mean of a and b 
    template <unsigned int N, class SCALAR>
    inline 
    SVectorWithError<N,SCALAR> average 
    ( const SVectorWithError<N,SCALAR>& v1 , 
      const SVectorWithError<N,SCALAR>& v2 ) { return v1.mean ( v2 ) ; }
    // ========================================================================
  } //                                             end of namespace Ostap::Math
  // ==========================================================================
} //                                                     end of namespace Ostap 
// ============================================================================
#include "SVectorWithError.icpp"
// ============================================================================
// The END
// ============================================================================
#endif // OSTAP_SVECTORWITHERROR_H
// ============================================================================
