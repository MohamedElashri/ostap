// ============================================================================
// Include files 
// ============================================================================
// STD&STL
// ============================================================================
#include <cmath>
#include <climits>
// ============================================================================
// Ostap
// ============================================================================
#include "Ostap/Kinematics.h"
#include "Ostap/PhaseSpace.h"
#include "Ostap/Tensors.h"
#include "Ostap/SymmetricMatrixTypes.h"
#include "Ostap/GenericMatrixTypes.h"
#include "Ostap/MoreMath.h"
// ============================================================================
// ROOT
// ============================================================================
#include "Math/Boost.h"
// ============================================================================
// Local
// ============================================================================
#include "local_math.h"
// ============================================================================
/** @file 
 *  Implementation file for functions from the file Ostap/Kinematics.h
 *  @date 2019-07-12 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
// ============================================================================
namespace 
{ 
  // ==========================================================================
  static_assert ( std::numeric_limits<float> ::is_specialized      ,
                  "std::numeric_limits<float>  is not specialized" ) ;
  /// large negative number 
  constexpr double s_INVALID = -0.9 * std::numeric_limits<float>::max () ;
  static_assert (  s_INVALID <  0   , "invalid negative number"    ) ;
  // ==========================================================================
}
// ============================================================================
/*  calculate the triangle function
 *  \f$ \lambda ( a , b, c ) = a^2 + b^2 + c^2 - 2ab - 2bc - 2 ca \f$
 *  @param a parameter a
 *  @param b parameter b
 *  @param c parameter b
 */
// ============================================================================
double Ostap::Kinematics::triangle
( const double a ,
  const double b ,
  const double c )
{ return a * a + b * b + c * c - 2 * a * b - 2 * b * c - 2 * a * c ; }
// ============================================================================
/*  universal four-particle kinematical function 
 *  @see E.Byckling, K.Kajantie, "Particle kinematics", John Wiley & Sons,
 *                  London, New York, Sydney, Toronto, 1973, p.89, eq. (5.23)
 *  @see https://userweb.jlab.org/~rafopar/Book/byckling_kajantie.pdf
 *  E.g. physical range for 2->2 scattering process is defined as
 * \f$ G(s,t,m_2^2, m_a^2, m_b^2, m_1^2) \le 0 \f$
 * or the physical range  for Dalitz plot is
 * \f$ G(s_2, s_1,  m_3^2, m_1^2, s , m_2^2) \le 0 \f$ 
 *
 * Actually the formula in E.Byckling & K.Kajantie  has a typo.
 * 
 * See the correct formula in: 
 * @see  P. Nyborg, H.S. Song, W. Kernan, R.H. Good,
 *       Phase-Space Considerations for Four-Particle Final States"
 *       Phys.Rev. 140 (1965) B914-B920, DOI: 10.1103/PhysRev.140.B914  
 * @see https://journals.aps.org/pr/pdf/10.1103/PhysRev.140.B914
 * @see http://inspirehep.net/record/49679?ln=en
 */ 
// ============================================================================
double Ostap::Kinematics::G 
( const double x , 
  const double y , 
  const double z , 
  const double u ,
  const double v , 
  const double w ) 
{
  return 
    x * x * y + x * y * y + 
    z * z * u + z * u * u +
    v * v * w + v * w * w + 
    x * z * w + x * u * v + 
    // y * z * w + y * u * w  // <--  typo in  E.Byckling & K.Kajantie
    y * z * v + y * u * w     // <--  correct line 
    - x * y * ( z + u + v + w ) 
    - z * u * ( x + y + v + w ) 
    - v * w * ( x + y + z + u ) ; 
}
// ============================================================================
/*  symmetric Gram determinant
 *  @see E.Byckling, K.Kajantie, "Particle kinematics", John Wiley & Sons,
 *              London, New York, Sydney, Toronto, 1973, p.89, eq. (5.23)
 *  @see https://userweb.jlab.org/~rafopar/Book/byckling_kajantie.pdf     
 */  
// ============================================================================
double Ostap::Kinematics::Gram::Delta  
( const Ostap::LorentzVector& p1 ) { return p1.M2() ; }
// ============================================================================
/* symmetric Gram determinant
 *  @see E.Byckling, K.Kajantie, "Particle kinematics", John Wiley & Sons,
 *              London, New York, Sydney, Toronto, 1973, p.89, eq. (5.23)
 *  @see https://userweb.jlab.org/~rafopar/Book/byckling_kajantie.pdf     
 */  
// ============================================================================
double Ostap::Kinematics::Gram::Delta
( const Ostap::LorentzVector& p1 , 
  const Ostap::LorentzVector& p2 ) 
{
  //  return -0.25 * Ostap::Kinematics::triangle 
  //  ( ( p1 + p2 ) . M2 () , p1.M2 () , p2. M2 () ) ;
  const long double p1p2 = p1 * p2 ;
  return p1.M2() * 1.0L * p2.M2() - p1p2 * p1p2 ;
}
// ============================================================================
/* symmetric Gram determinant
 *  @see E.Byckling, K.Kajantie, "Particle kinematics", John Wiley & Sons,
 *              London, New York, Sydney, Toronto, 1973, p.89, eq. (5.23)
 *  @see https://userweb.jlab.org/~rafopar/Book/byckling_kajantie.pdf     
 */
// ============================================================================
double Ostap::Kinematics::Gram::Delta
( const Ostap::LorentzVector& p1 , 
  const Ostap::LorentzVector& p2 ,
  const Ostap::LorentzVector& p3 ) 
{
  return -0.125 * Ostap::Kinematics::G 
    ( ( p1 + p2      ) . M2 () ,
      ( p1      - p3 ) . M2 () , 
      ( p1 + p2 - p3 ) . M2 () , 
      p1 . M2 () ,
      p2 . M2 () , 
      p3 . M2 () ) ;
}
// ============================================================================
/*  symmetric Gram determinant
 *  \f$ Delta( p_1, p_2, p_3, p_4 ) \equiv 
 *   G \left( \begin{array}{llll}p_1,& p_2, &p_3,&p_4 \\ 
 *                               p_1,& p_2, &p_3,&p_4 \end{array} \right) \f$ 
 *  @see E.Byckling, K.Kajantie, "Particle kinematics", John Wiley & Sons,
 *              London, New York, Sydney, Toronto, 1973, p.89, eq. (5.23)
 *  @see https://userweb.jlab.org/~rafopar/Book/byckling_kajantie.pdf     
 */
// ============================================================================
double Ostap::Kinematics::Gram::Delta
( const Ostap::LorentzVector& p1 , 
  const Ostap::LorentzVector& p2 ,
  const Ostap::LorentzVector& p3 ,
  const Ostap::LorentzVector& p4 ) 
{
  ROOT::Math::SMatrix<long double,4,4,ROOT::Math::MatRepSym<long double,4> >  mtrx ;
  //
  mtrx ( 0 , 0 ) = p1 * p1 ;
  mtrx ( 0 , 1 ) = p1 * p2 ;
  mtrx ( 0 , 2 ) = p1 * p3 ;
  mtrx ( 0 , 3 ) = p1 * p4 ;
  //
  mtrx ( 1 , 1 ) = p2 * p2 ;
  mtrx ( 1 , 2 ) = p2 * p3 ;
  mtrx ( 1 , 3 ) = p2 * p4 ;
  //
  mtrx ( 2 , 2 ) = p3 * p3 ;
  mtrx ( 2 , 3 ) = p3 * p4 ;
  //
  mtrx ( 3 , 3 ) = p4 * p4 ;
  //
  long double det = 0 ;
  mtrx.Det( det ) ;
  //
  return det ;
}
// ============================================================================
/*  non-symmetric Gram determinant
 *  \f[ G \left(\begin{array}{ll} p_1 , & p_2 \\
 *                                q_1 , & q_2 \end{array}\right) 
 *  = \left| \begin{array}{ll}
 *  p_1q_1 & p_1q_2 \\
 *  p_2q_1 & p_2q_2 \end{array} \right| \f]
 */
// =============================================================================
double Ostap::Kinematics::Gram::G 
( const Ostap::LorentzVector& p1 , 
  const Ostap::LorentzVector& p2 ,
  const Ostap::LorentzVector& q1 , 
  const Ostap::LorentzVector& q2 ) 
{ 
  const double a11 = p1 * q1 ;
  const double a12 = p1 * q2 ;
  const double a21 = p2 * q1 ;
  const double a22 = p2 * q2 ;
  //
  return a11 * a22 - a12 * a21 ;
}
// ============================================================================
/*  non-symmetric Gram determinant
 *  \f[ G \left(\begin{array}{lll} p_1 , & p_2, & p_3 \\
 *                                 q_1 , & q_2, & q_3 \end{array}\right) 
 *  = \left| \begin{array}{lll}
 *  p_1q_1 & p_1q_2 & p_1q_3 \\
 *  p_2q_1 & p_2q_2 & p_2q_3 \\
 *  p_3q_1 & p_3q_2 & p_3q_3 \end{array} \right| \f]
 */
// ============================================================================
double Ostap::Kinematics::Gram::G 
( const Ostap::LorentzVector& p1 , 
  const Ostap::LorentzVector& p2 ,
  const Ostap::LorentzVector& p3 ,
  const Ostap::LorentzVector& q1 ,
  const Ostap::LorentzVector& q2 ,
  const Ostap::LorentzVector& q3 ) 
{
  //
  const long double a11 = p1 * q1 ;
  const long double a12 = p1 * q2 ;
  const long double a13 = p1 * q3 ;
  const long double a21 = p2 * q1 ;
  const long double a22 = p2 * q2 ;
  const long double a23 = p2 * q3 ;
  const long double a31 = p3 * q1 ;
  const long double a32 = p3 * q2 ;
  const long double a33 = p3 * q3 ;
  //
  return 
    a11 * a22 * a33 +
    a12 * a23 * a21 + 
    a13 * a32 * a21 -
    a13 * a22 * a31 - 
    a21 * a12 * a33 - 
    a11 * a23 * a32 ;  
}
// ============================================================================
/*  non-symmetric Gram determinant
 *  \f[ G \left(\begin{array}{llll} p_1 , & p_2,& p_3, &p_4 \\
 *                                 q_1 , & q_2, & q_3, &q_4 \end{array}\right) 
 *  = \left| \begin{array}{llll}
 *  p_1q_1 & p_1q_2 & p_1q_3 & p_1q_4\\
 *  p_2q_1 & p_2q_2 & p_2q_3 & p_2q_4\\
 *  p_3q_1 & p_3q_2 & p_3q_3 & p_3q_4\\ 
 *  p_4q_1 & p_4q_2 & p_4q_3 & p_4q_4\end{array} \right| \f]
 */
// ============================================================================
double Ostap::Kinematics::Gram::G 
( const Ostap::LorentzVector& p1 , 
  const Ostap::LorentzVector& p2 ,
  const Ostap::LorentzVector& p3 ,
  const Ostap::LorentzVector& p4 ,
  const Ostap::LorentzVector& q1 ,
  const Ostap::LorentzVector& q2 ,
  const Ostap::LorentzVector& q3 ,
  const Ostap::LorentzVector& q4 ) 
{
  //
  ROOT::Math::SMatrix<long double,4,4> mtrx ;
  //
  mtrx ( 0 , 0 ) = p1 * q1 ;
  mtrx ( 0 , 1 ) = p1 * q2 ;
  mtrx ( 0 , 2 ) = p1 * q3 ;
  mtrx ( 0 , 3 ) = p1 * q4 ;
  //
  mtrx ( 1 , 0 ) = p2 * q1 ;
  mtrx ( 1 , 1 ) = p2 * q2 ;
  mtrx ( 1 , 2 ) = p2 * q3 ;
  mtrx ( 1 , 3 ) = p2 * q4 ;
  //
  mtrx ( 2 , 0 ) = p3 * q1 ;
  mtrx ( 2 , 1 ) = p3 * q2 ;
  mtrx ( 2 , 2 ) = p3 * q3 ;
  mtrx ( 2 , 3 ) = p3 * q4 ;
  //
  mtrx ( 3 , 0 ) = p4 * q1 ;
  mtrx ( 3 , 1 ) = p4 * q2 ;
  mtrx ( 3 , 2 ) = p4 * q3 ;
  mtrx ( 3 , 3 ) = p4 * q4 ;
  //
  long double det = 0 ;
  mtrx.Det( det ) ;
  //
  return det ;
}
// ============================================================================
/*  Boost LorentzVector into rest-frame of another Lorentz vector 
 *  @param what   the vextro to be bosted 
 *  @param frame  the 4-vector of the frame 
 *  @return boosted vector 
 */
// ============================================================================
Ostap::LorentzVector
Ostap::Kinematics::boost 
( const Ostap::LorentzVector& what  ,
  const Ostap::LorentzVector& frame )
{
  const ROOT::Math::Boost b { frame.BoostToCM() } ;
  return b ( what ) ;
}
// ============================================================================
/*  simple function which evaluates the magnitude of 3-momentum
 *  of particle "v" in the rest system of particle "M"
 *
 *  \f$ \left|\vec{p}\right|
 *     \sqrt{  \frac{\left(v\cdot M\right)^2}{M^2} -v^2} \f$
 *
 *  @attention particle M must be time-like particle!
 *  @param v the vector to be checked
 *  @param M the defintion of "rest"-system
 *  @return the magnitude of 3D-momentum of v in rest-frame of M
 *  @date 2008-07-27
 */
// ============================================================================
double Ostap::Kinematics::restMomentum
( const Ostap::LorentzVector& v ,
  const Ostap::LorentzVector& M )
{
  const double M2 = M.M2 ( ) ;
  if ( 0 >= M2 ) { return s_INVALID ; } //   ATTENTION!
  const double vM = v.Dot(M) ;
  const double P2 = vM * vM / M2 - v.M2() ;
  if ( 0 >  P2 ) { return s_INVALID ; } //   ATTENTION!
  return 0 <= P2 ? std::sqrt ( P2 ) : -std::sqrt ( std::abs ( P2 ) ) ;
}
// ============================================================================
/*  simple function which evaluates the energy
 *  of particle "v" in the rest system of particle "M"
 *
 *  \f$ e = \frac{v\cdot M}{\sqrt{M^2}} \f$
 *
 *  @attention particle M must be time-like particle: M^2 > 0 !
 *  @param v the vector to be checked
 *  @param M the defintion of "rest"-system
 *  @return the energy of v in rest-frame of M
 *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
 *  @date 2008-07-27
 */
// ============================================================================
double Ostap::Kinematics::restEnergy
( const Ostap::LorentzVector& v ,
  const Ostap::LorentzVector& M )
{
  const double M2 = M.M2 () ;
  if ( 0 >= M2 ) { return s_INVALID ;  } //  RETURN 
  // evaluate the energy
  return v.Dot( M ) / std::sqrt ( M2 ) ;
}
// ============================================================================
/*  simple function for evaluation of the euclidian norm
 *  for LorentzVectors
 *  (E**2+Px**2+Py**2+Pz**2)
 *  @param vct the vector
 *  @return euclidian norm squared
 *  @date 2006-01-17
 */
// ============================================================================
double Ostap::Kinematics::euclidianNorm2 
( const Ostap::LorentzVector& vct )
{
  return
    vct.e() * vct.e() +
    vct.x() * vct.x() +
    vct.y() * vct.y() +
    vct.z() * vct.z() ;
}
// ============================================================================
/*  simple function which evaluates the transverse
 *  momentum with respect a certain 3D-direction
 *  @param mom the momentum
 *  @param dir the direction
 *  @return the transverse momentum with respect to the direction
 *  @author Vanya BELYAEV Ivan.Belayev@itep.ru
 *  @date 2006-01-17
 */
// ============================================================================
double Ostap::Kinematics::transverseMomentumDir
( const Ostap::Vector3D& mom , 
  const Ostap::Vector3D& dir ) 
{
  const double dmag2 = dir.Mag2();
  if ( 0 == dmag2 ) { return mom.R(); }
  const Ostap::Vector3D perp = mom - dir * ( mom.Dot( dir ) / dmag2 );
  return perp.R();
}
// ============================================================================
/*  This routine returns the cosine angle theta
 *  The decay angle calculated  is that between
 *  the flight direction of the daughter meson, "D",
 *  in the rest frame of "Q" (the parent of "D"),
 *  with respect to "Q"'s flight direction in "P"'s
 *  (the parent of "Q") rest frame
 *
 *  \f[
 *  \cos \theta = \frac
 *  { \left(P \cdot D\right)Q^2 -
 *    \left(P \cdot Q\right)\left(D \cdot Q \right) }
 *  {\sqrt{ \left[ \left( P \cdot Q \right)^2 - Q^2 P^2 \right]
 *          \left[ \left( D \cdot Q \right)^2 - Q^2 D^2 \right] } }
 *  \f]
 *
 *  Note that the expression has the symmetry: \f$ P \leftrightarrow D \f$
 *
 *  Essentially it is a rewritten <c>EvtDecayAngle(P,Q,D)</c>
 *  routine from EvtGen package
 *
 *  @param D 4-momentum of the daughter particle
 *  @param Q 4-momentum of mother particle
 *  @param P "rest frame system"
 *  @return cosine of decay angle
 *
 *  @see Ostap::LorentzVector
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2004-12-03
 */
// ===========================================================================
double Ostap::Kinematics::decayAngle 
( const Ostap::LorentzVector& P , 
  const Ostap::LorentzVector& Q ,
  const Ostap::LorentzVector& D ) 
{
  //
  const double pd  = P.Dot ( D ) ; // P * D
  const double pq  = P.Dot ( Q ) ; // P * Q
  const double qd  = Q.Dot ( D ) ; // D * Q
  const double mq2 = Q.M2  (   ) ; // Q^2
  const double mp2 = P.M2  (   ) ; // P^2
  const double md2 = D.M2  (   ) ; // D^2
  //
  const double value = ( pq * pq - mq2 * mp2 ) * ( qd * qd - mq2 * md2 ) ;
  //
  return 0 <= value ? ( pd * mq2 - pq * qd ) / std::sqrt( value ) : s_INVALID ;
}
// ============================================================================
/*  This routine returns the cosine angle theta
 *  The decay angle calculated  is that between
 *  the flight direction of the daughter meson, "D",
 *  in the rest frame of "M" (the parent of "D"),
 *  with respect to the boost direction from
 *  "M"'s rest frame
 *
 *  @param D 4-momentum of the daughter particle
 *  @param M 4-momentum of mother particle
 *  @return cosine of decay angle
 *
 *  Clearly it is a variant of 3-argument with the
 *  P-argument to be of type (0,0,0,E)
 *  (=="laborator frame")
 *
 *  @see Ostap::LorentzVector
 *  @see Ostap::Kinematics::decayAngle 
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2004-12-03
 */
// ============================================================================
double Ostap::Kinematics::decayAngle
( const Ostap::LorentzVector& D , 
  const Ostap::LorentzVector& M ) 
{ return decayAngle( Ostap::LorentzVector ( 0, 0, 0, 10 * M.E() ) , M , D ); }
// ============================================================================
/*  simple function to evaluate the cosine angle between
 *  two directions (v1 and v2) in the rest system of M
 *
 * \f[
 * \cos\theta =
 * \frac{\vec{p}_1\vec{p}_2}{\left|\vec{p}_1\right|
 * \left|\vec{p}_2\right|} =
 * \frac{1}{\left|\vec{p}_1\right|\left|\vec{p}_2\right|}
 * \left( E_1E_2 -\frac{1}{2}
 * \left(\left(v_1+v_2\right)^2-v_1^2-v_2^2 \right) \right),
 * \f]
 *
 *  where
 *  \f[
 *  E_1 E_2 = \frac{ \left ( v_1 \cdot M\right) \left (v_2 \cdot M \right ) }{M^2}
 *  \f]
 *  and
 *  \f[
 * \left|\vec{p}_1\right|\left|\vec{p}_2\right| =
 * \sqrt{
 * \left( \frac{\left(v_1\cdot M\right)^2}{M^2}-v_1^2 \right)
 *      \left( \frac{\left(v_2\cdot M\right)^2}{M^2}-v_2^2 \right) }
 * \f]
 *
 *  Note that the expressions are clear Lorentz invariant
 *
 *  @attention the particle M must be time-like particle: M^2 > 0 !
 *  @param v1 the first vector
 *  @param v2 the last vector
 *  @param M  the defintion of rest-system
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2008-07-27
 */
// ============================================================================
double Ostap::Kinematics::cosThetaRest
( const Ostap::LorentzVector& v1 , 
  const Ostap::LorentzVector& v2 ,
  const Ostap::LorentzVector& M  ) 
{
  //
  const double M2 = M.M2();
  if ( 0 >= M2 ) { return s_INVALID ; }     // RETURN
  ///
  const double v1M  = v1.Dot( M );
  const double v2M  = v2.Dot( M );
  const double m1_2 = v1.M2();
  const double m2_2 = v2.M2();
  
  // calculate e1*e2
  const double e1e2 = v1M * v2M / M2; // calculate e1*e2
  
  // calculate (|p1|*|p2|)^2
  const double p1p2_ = ( ( v1M * v1M ) / M2 - m1_2 ) * ( ( v2M * v2M ) / M2 - m2_2 ); // calculate (|p1|*|p2|)^2
  //
  if ( 0 >= p1p2_ ) { return s_INVALID ; }   // RETURN
  //
  const double p1p2 = ::sqrt( p1p2_ ); // |p1|*|p2|
  //
  const double var = ( v1 + v2 ).M2() - m1_2 - m2_2;
  //
  // finally evaluate the cosine
  return ( e1e2 - 0.5 * var ) / p1p2;
}
// ============================================================================
/*  Cosine of the angle between p1 and p2 in the rest frame of M
 *  \f$ \cos \theta = - \frac 
 *  { G   \left( \begin{array}{ll} M, &p_1 \\ M,& p_2 \end{array}\right) }
 *  { \left\[ \Delta_2(M,p_1)\Delta_2(M,p_2)\right\]^{1/2} }\f$  
 *  @param v1 the first vector
 *  @param v2 the last vector
 *  @param M  the definition of rest-system
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2019-07-17
 */
// ============================================================================
double Ostap::Kinematics::cos_theta 
( const Ostap::LorentzVector& p1 ,
  const Ostap::LorentzVector& p2 ,
  const Ostap::LorentzVector& M  ) 
{
  //
  const double f1 = Ostap::Kinematics::Gram::Delta ( M , p1 ) ;
  if ( f1 >= 0 ) { return s_INVALID ; }
  //
  const double f2 = Ostap::Kinematics::Gram::Delta ( M , p2 ) ;
  if ( f2 >= 0 ) { return s_INVALID ; }
  //
  const double g =  Ostap::Kinematics::Gram::G ( M , p1 , M , p2 ) ;
  //
  return -g / std::sqrt ( f1 * f2 ) ;
}
// ============================================================================
/*  Sine squared  of the angle between p1 and p2 in the rest frame of M
 *  \f$ \sin^2 \theta = \frac{ \Delta ( M ) \Delta ( M , p_1 , p_2 ) }
 *  { \Delta ( M, p_1  ) \Delta ( M , p_2 ) } \f$ 
 *  @param v1 the first vector
 *  @param v2 the last vector
 *  @param M  the defintion of rest-system
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2019-07-17
 */
// ============================================================================
double Ostap::Kinematics::sin2_theta 
( const Ostap::LorentzVector& p1 ,
  const Ostap::LorentzVector& p2 ,
  const Ostap::LorentzVector& M  )
{
  return 
    Ostap::Kinematics::Gram::Delta ( M           ) * 
    Ostap::Kinematics::Gram::Delta ( M , p1 , p2 ) / 
    ( Ostap::Kinematics::Gram::Delta ( M , p1 ) * 
      Ostap::Kinematics::Gram::Delta ( M , p2 ) ) ; 
}
// ============================================================================
/*  evaluate the angle \f$\chi\f$
 *  beween two decay planes,
 *  formed by particles v1&v2 and h1&h2 correspondingly.
 *  The angle is evaluated in the rest frame
 *  of "mother" particles (defined as v1+v2+h1+h2)
 *
 *  @param d1 the 1st daughter
 *  @param d2 the 2nd daughter
 *  @param h1 the 3rd daughter
 *  @param h2 the 4th daughter
 *  @return angle chi
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2008-07-27
 */
// ============================================================================
double Ostap::Kinematics::decayAngleChi 
( const Ostap::LorentzVector& d1 , 
  const Ostap::LorentzVector& d2 ,
  const Ostap::LorentzVector& h1 , 
  const Ostap::LorentzVector& h2 )
{
  //
  const double cosChi = cosDecayAngleChi ( d1 , d2 , h1 , h2 ) ;
  if ( std::abs ( cosChi ) > 1  ) { return s_INVALID ; }
  //
  const double sinChi = sinDecayAngleChi ( d1 , d2 , h1 , h2 ) ;
  if ( std::abs ( sinChi ) > 1  ) { return s_INVALID ; }
  //
  return std::atan2 ( sinChi , cosChi ) ;
}
// ============================================================================
/*  evaluate \f$\cos \chi\f$, where \f$\chi\f$ if the angle
 *  beween two decay planes, formed by particles d1&d2
 *  and h1&h2 correspondingly.
 *
 *  The angle is evaluated in the rest frame
 *  of "mother" particles (defined as d1+d2+h1+h2)
 *
 *  The angle is evaluated using the explicit
 *  Lorenzt-invariant expression:
 *  \f[
 *  \cos \chi =
 *   - \frac{ L_D^{\mu} L_H^{\mu} }
 *     { \sqrt{ \left[ -L_D^2 \right]\left[ -L_H^2 \right] }},
 &   =
 *   - \frac{
 *     \epsilon_{ijkl}d_1^{j}d_2^{k}\left(h_1+h_2\right)^l
 *     \epsilon_{imnp}h_1^{m}h_2^{n}\left(d_1+d_2\right)^p }
 *     { \sqrt{ \left[ -L_D^2 \right]\left[ -L_H^2 \right] }},
 *  \f]
 *  where "4-normales" are defined as:
 *  \f[
 *   L_D^{\mu} = \epsilon_{\mu\nu\lambda\kappa}
 *                d_1^{\nu}d_2^{\lambda}\left(h_1+h_2\right)^{\kappa}
 *  \f]
 *   and
 *  \f[
 *   L_H^{\mu} = \epsilon_{\mu\lambda\delta\rho}
 *                h_1^{\lambda}h_2^{\delta}\left(d_1+d_2\right)^{\rho}
 *   \f]
 *
 *  @param d1 the 1st daughter
 *  @param d2 the 2nd daughter
 *  @param h1 the 3rd daughter
 *  @param h2 the 4th daughter
 *  @return cos(chi)
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2008-07-27
 */
// ============================================================================
double Ostap::Kinematics::cosDecayAngleChi
( const Ostap::LorentzVector& d1 , 
  const Ostap::LorentzVector& d2 ,
  const Ostap::LorentzVector& h1 , 
  const Ostap::LorentzVector& h2 ) 
{
  // get the intermediate particles D & H
  const Ostap::LorentzVector D( d1 + d2 );
  const Ostap::LorentzVector H( h1 + h2 );
  //
  // Evaluator of various tensor expressions:
  Ostap::Math::Tensors::Epsilon e;
  //
  // evaluate the length of normales :
  const double l1 = e.mag2( d1, d2, H ); // == | [d1,d2,M] |
  const double l2 = e.mag2( h1, h2, D ); // == | [h1,h2,M] |
  //
  if ( 0 <= l1 ) { return s_INVALID ; }
  if ( 0 <= l2 ) { return s_INVALID ; }
  //
  return -e.epsilon( d1, d2, H, h1, h2, D ) / std::sqrt( l1 * l2 );
}
// ============================================================================
/*  evaluate \f$\sin\chi\f$, where \f$\chi\f$ is the angle
 *  beween two decay planes,
 *  formed by particles v1&v2 and h1&h2 correspondingly.
 *  The angle is evaluated in the rest frame
 *  of "mother" particles (defined as v1+v2+h1+h2)
 *
 *  The angle is  calculated using the explicit
 *   Lorentz-invariant expression:
 *  \f[
 *   \sin \chi =
 *   \frac  {
 *   \epsilon_{\mu\nu\lambda\delta}
 *   L_D^{\mu}L_H^{\nu}H^{\lambda}M^{\delta} }
 *   { \sqrt{
 *   \left[ -L_D^2 \right]\left[ -L_H^2 \right]
 *   \left[ \left( H\ cdot M\right)^2-H^2M^2 \right]
 *   }} = \frac {
 *   \epsilon_{\mu\nu\lambda\delta}
 *   d_1^{\mu}d_2^{\nu}h_1^{\lambda}h_2^{\delta}
 *   \left( \left( D \cdot H \right)^2 - D^2H^2 \right) }
 *   { \sqrt{
 *   \left[ -L_D^2 \right]\left[ -L_H^2    \right]
 *   \left[ \left(H\cdot M\right)^2-H^2M^2 \right]
 *   }},
 *  \f]
 *  where "4-normales" are defined as:
 *  \f$
 *  L_D^{\mu} = \epsilon_{\mu\nu\lambda\kappa}
 *                d_1^{\nu}d_2^{\lambda}\left(h_1+h_2\right)^{\kappa}
 *  \f$,
 *  \f$
 *  L_H^{\mu} = \epsilon_{\mu\lambda\delta\rho}
 *  h_1^{\lambda}h_2^{\delta}\left(d_1+d_2\right)^{\rho}
 *  \f$
 *  and   \f$ D = d_1 + d_2 \f$,
 *        \f$ H = h_1 + h_2 \f$,
 *        \f$ M = D + H = d_1 + d_2 + h_1+h_2 \f$.
 *
 *  @param d1 the 1st daughter
 *  @param d2 the 2nd daughter
 *  @param h1 the 3rd daughter
 *  @param h2 the 4th daughter
 *  @return angle chi
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2008-07-27
 */
// ============================================================================
double Ostap::Kinematics::sinDecayAngleChi 
( const Ostap::LorentzVector& d1 , 
  const Ostap::LorentzVector& d2 ,
  const Ostap::LorentzVector& h1 , 
  const Ostap::LorentzVector& h2 ) 
{
  // ========================================================================
  //  reconstrcut the intermediate particles
  const Ostap::LorentzVector D( d1 + d2 );
  const Ostap::LorentzVector H( h1 + h2 );
  /// Mother Particle
  const Ostap::LorentzVector M( D + H );
  // ========================================================================
  const double M2 = M.M2();
  if ( 0 >= M2 ) { return s_INVALID ; }
  // ========================================================================
  // evaluator of various tensor expressions
  Ostap::Math::Tensors::Epsilon e;
  // ========================================================================
  // get the length of 4-normales
  const double l1 = e.mag2( d1, d2, H ); // == | [d1,d2,M] |
  const double l2 = e.mag2( h1, h2, D ); // == | [h1,h2,M] |
  //
  if ( 0 <= l1 ) { return s_INVALID ; }
  if ( 0 <= l2 ) { return s_INVALID ; }
  //
  // try to get the sine
  //
  const double DH = D.Dot( H );
  //
  const double var = e( d1, d2, h1, h2 ) * ( DH * DH - D.M2() * H.M2() );
  //
  const double HM  = H.Dot( M );
  const double p_H = HM * HM - H.M2() * M2;
  //
  if ( 0 >= p_H ) { return s_INVALID ; }
  //
  return var / std::sqrt( l1 * l2 * p_H );
}
// ============================================================================
/*  evaluate the Armenteros-Podolanski variable \f$\mathbf{\alpha}\f$,
 *  defined as:
 *  \f[
 *  \mathbf{\alpha} = \frac
 *  { \mathrm{p}^{\mathrm{L},1} - \mathrm{p}^{\mathrm{L},1} }
 *  { \mathrm{p}^{\mathrm{L},1} + \mathrm{p}^{\mathrm{L},1} },
 *  \f]
 *  where
 *   \f$ \mathrm{p}^{\mathrm{L},1}\f$ and
 *   \f$ \mathrm{p}^{\mathrm{L},2}\f$ are longitudinal momentum
 *   components for the first and the second daughter particles
 *   with respect to the total momentum direction.
 *
 *  Clearly this expression could be rewritten in an equivalent
 *  form which however much more easier for calculation:
 *  \f[
 *  \mathbf{\alpha} = \frac
 *  { \vec{\mathbf{p}}_1^2 - \vec{\mathbf{p}}_2^2 }
 *  { \left( \vec{\mathbf{p}}_1 + \vec{\mathbf{p}}_2 \right)^2 }
 *  \f]
 *
 *  @attention instead of
 *     2D \f$\left(\mathrm{p_T},\mathbf{\alpha}\right)\f$ diagram,
 *     in the case of twobody decays at LHCb it is much better to
 *     use 2D diagram \f$\left(\cos \theta, \mathrm{m} \right)\f$
 *     diagram, where \f$\cos\theta\f$-is the decay
 *     angle,see the variable LV01, and \f$\mathrm{m}\f$ is an
 *     invariant evalauted for some (fixed) mass prescription,
 *     e.g. \f$\pi^+\pi^-\f$.
 *
 *  @param d1  three momentum of the first  daughter particle
 *  @param d2  three momentum of the second daughter particle
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2008-09-21
 */
// ============================================================================
double Ostap::Kinematics::armenterosPodolanskiX 
( const Ostap::Vector3D& d1 , 
  const Ostap::Vector3D& d2 )
{ return ( d1.Mag2() - d2.Mag2() ) / ( d1 + d2 ).Mag2(); }
// ============================================================================
/*  trivial functon to get the component of "a", transverse to "b"
 *  @param a (INPUT)  three vector
 *  @param b (INPUT)  reference direction
 *  @return component of "a", transverse to "b"
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-02-04
 */
// ============================================================================
Ostap::Vector3D Ostap::Kinematics::transverse
( const Ostap::ThreeVector& a, 
  const Ostap::ThreeVector& b )
{ return   a -  parallel ( a  , b ) ; }
// ============================================================================
/* trivial function to get the component of "a" parallel to "b"
 *  @param a (INPUT)  three vector
 *  @param b (INPUT)  reference direction
 *  @return component of "a" parallel to "b"
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-02-04
 */
// ============================================================================
Ostap::Vector3D Ostap::Kinematics::parallel
( const Ostap::Vector3D& a  , 
  const Ostap::Vector3D& b  ) 
{
  //
  const double ab = a.Dot( b );
  const double b2 = b.Mag2();
  if ( 0 >= b2 ) { return a ; }
  //
  Ostap::Vector3D r( b );
  r *= ( ab / b2 );
  return r;
}
// ============================================================================
/*  momentum of the first partle form two-body decay
 *  \f$ m\rightarrow m_1 m_2 \f$ in th eret frasme of \f$ m \f$.
 *  \f[ q ( m , m_1 , m_2 )  \equiv 
 *  \frac{\lambda^{1/2}\left( m^2, m_1^2, m_2^2\right)}{2m} \f]
 */
// ============================================================================
double Ostap::Kinematics::q 
( const double m  , 
  const double m1 ,
  const double m2 ) 
{
  if ( m <= m1 + m2 || m1 < 0 || m2 < 0 ) { return 0 ; }
  //
  const double l = triangle  ( m * m , m1 * m1 , m2 * m2 ) ;
  //
  return 0 < l ?  0.5 * std::sqrt ( l ) / m : 0.0 ;
}
// ============================================================================
/*  momentum of the first particle from two-body decay
 *  \f$ \sqrt{s} \rightarrow m_1 m_2 \f$ 
 *  in the rest frame of the mother particle \f$ \sqrt{s}\f$.
 *  \f[ q_s ( s , m_1^2 , m_2^2 )  \equiv 
 *  \frac{\lambda^{1/2}\left( s , m_1^2, m_2^2\right)}{2 \sqrt{s} } \f]
 *  @param s     squared mass of the mother particle 
 *  @param m2_1  squared mass of the first  particle 
 *  @param m2_2  squared mass of the second particle 
 */
// ============================================================================
double Ostap::Kinematics::q_s
( const double s    , 
  const double m2_1 ,
  const double m2_2 ) 
{
  if ( s <= 0 || m2_1 < 0 || m2_2 < 0 ) { return 0 ; }
  //
  const double l = triangle  ( s , m2_1 , m2_2 ) ;
  //
  return 0 < l ?  0.5 * std::sqrt ( l / s ) : 0.0 ;
}
// ============================================================================
/*  two-body phase space 
 *  \f[ \Phi_2( m ) = \dfrac{1}{8\pi} \dfrac{ \sqrt{\lambda \left( m^2 , m_1^2, m_2^2 \right) }}{m^2} \f] 
 * Note that there exist also an alternative normalization:
 *  \f[ \Phi_2^{\prime}( m ) = \dfrac{\pi}{2} 
 *   \dfrac{ \sqrt{\lambda \left( m^2 , m_1^2, m_2^2 \right) }}{m^2} \f],
 *  this one is used e.g. in  
 *  E.Byckling, K.Kajantie, "Particle kinematics", John Wiley & Sons,
 *              London, New York, Sydney, Toronto, 1973, Eq. (V.1.9)
 *  @see Ostap::Kinematics::phasespace2_bk 
 */
// ============================================================================
double Ostap::Kinematics::phasespace2
( const double x  , 
  const double m1 , 
  const double m2 ) 
{
  //
  const double xm1 = std::max ( 0.0 , m1 ) ;
  const double xm2 = std::max ( 0.0 , m2 ) ;
  //
  if ( x <= xm1 + xm2 ) { return 0 ; } //  RETURN 
  //
  const double msq = x * x ;
  const double lam = Ostap::Kinematics::triangle ( msq , xm1 * xm1 , xm2 * xm2 ) ;
  //
  static const double s_inv8pi = 1.0 / ( 8 * M_PI ) ;
  //
  return 0 < lam ? s_inv8pi * std::sqrt ( lam ) / msq : 0.0 ;
}
// ============================================================================
/* two-body phase space: 
 *  \f[ \Phi_2^{\prime}( m ) = \dfrac{\pi}{2} 
 *   \dfrac{ \sqrt{\lambda \left( m^2 , m_1^2, m_2^2 \right) }}{m^2} \f],
 *  This one is used e.g. in  
 *  E.Byckling, K.Kajantie, "Particle kinematics", John Wiley & Sons,
 *              London, New York, Sydney, Toronto, 1973, Eq. (V.1.9)
 * Note that there exist also an alternative normalization:
 *  \f[ \Phi_2( m ) = \dfrac{1}{8\pi} 
 *   \dfrac{ \sqrt{\lambda \left( m^2 , m_1^2, m_2^2 \right) }}{m^2} \f]
 *  @see Ostap::Kinematics::phasespace2_bk 
 */
// ============================================================================
double Ostap::Kinematics::phasespace2_bk 
( const double x  , 
  const double m1 , 
  const double m2 ) 
{
  const double xm1 = std::max ( 0.0 , m1 ) ;
  const double xm2 = std::max ( 0.0 , m2 ) ;
  //
  if ( x <= xm1 + xm2 ) { return 0 ; } //  RETURN 
  //
  const double msq = x * x ;
  const double lam = Ostap::Kinematics::triangle ( msq , xm1 * xm1 , xm2 * xm2 ) ;
  //
  static const double s_norm = 0.5 * M_PI ;  
  //
  return 0 < lam ? s_norm * std::sqrt ( lam ) / msq : 0.0 ;
}
// ============================================================================
/** three-body phase space 
 */
// ============================================================================
#include <iostream> 
double Ostap::Kinematics::phasespace3 
( const double x  , 
  const double m1 , 
  const double m2 , 
  const double m3 ) 
{
  //
  static const double s_norm = 0.125 * M_PI * M_PI ;
  //
  double xm1 = std::max ( 0.0 , m1 ) ;
  double xm2 = std::max ( 0.0 , m2 ) ;
  double xm3 = std::max ( 0.0 , m3 ) ;
  //
  if ( x <= xm1 + xm2 + xm3 ) { return 0 ; } //   RETURN
  //
  const bool zero1 = s_zero ( xm1 ) ; xm1 = zero1 ? 0.0 : xm1 ;
  const bool zero2 = s_zero ( xm2 ) ; xm2 = zero2 ? 0.0 : xm2 ;   
  const bool zero3 = s_zero ( xm3 ) ; xm3 = zero3 ? 0.0 : xm3 ;   
  //
  if ( x <= xm1 + xm2 + xm3 ) { return 0 ; } //   RETURN
  //
  // make the proper ordering  
  if ( xm1 < xm2 || xm2 < xm3 )
  {
    if ( xm1 < xm2 ) { std::swap ( xm1 , xm2 ) ; }
    if ( xm2 < xm3 ) { std::swap ( xm2 , xm3 ) ; if ( xm1 < xm2 ) { std::swap ( xm1 , xm2 ) ; } } 
    return phasespace3 ( x , xm1 , xm2 , xm3 ) ;  
  }  
  //
  // 1) all masses are zero 
  if      (  zero1 &&  zero2 && zero3 ) { return s_norm * x * x ; } // all masses are zero 
  // 2) one mass is zero 
  else if ( !zero1 && !zero2 && zero3 )  
  {    
    // E.Byckling, K.Kajantie V.5 
    const double m1_2 = xm1 * xm1 ;
    const double m2_2 = xm2 * xm2 ;
    // 
    const double summ2 =            m1_2 + m2_2   ;
    const double difm2 = std::abs ( m1_2 - m2_2 ) ;
    //
    const double s     = x * x ;
    const double sqlam = std::sqrt ( Ostap::Kinematics::triangle ( s , m1_2 , m2_2 ) ) ;
    //
    double res   = sqlam * ( s + summ2 ) / ( s * s ) ;
    res += 2 * difm2 * std::log ( ( s * summ2 - difm2 * difm2  + difm2 * sqlam )    / ( 2 * xm1  * xm2 * s ) ) / s ;
    res -= 2 * ( s * summ2 - 2  * m1_2 * m2_2 ) * std::log ( ( s - summ2  + sqlam ) / ( 2 * xm1 * xm2      ) ) / ( s * s ) ;
    //
    return s_norm * s * res ;  
  }
  // 3) two masses are zero 
  else if ( !zero1 && zero2 && zero3 ) 
  {
    // E.Byckling, K.Kajantie V.5 
    const double m1_2 = xm1 * xm1 ;
    // 
    const double summ2 = m1_2 ;
    const double difm2 = m1_2 ;
    //
    const double s     = x * x ;
    const double sqlam = std::sqrt ( Ostap::Kinematics::triangle ( s , m1_2 , 0 ) ) ;
    //
    double res   = sqlam * ( s + summ2 ) / ( s * s ) ;
    res += 2 * m1_2 * std::log ( m1_2 / s ) / s ;
    //
    return s_norm * s * res ;
  }
  //
  //
  const double p2    = x * x ;
  const double M     = x     ;
  //
  const double Qp = 
    ( M + xm1 + xm2 + xm3 ) *
    ( M + xm1 - xm2 - xm3 ) *
    ( M - xm1 + xm2 - xm3 ) *
    ( M - xm1 - xm2 + xm3 ) ;
  //
  const double Qm = 
    ( M - xm1 - xm2 - xm3 ) *
    ( M - xm1 + xm2 + xm3 ) *
    ( M + xm1 - xm2 + xm3 ) *
    ( M + xm1 + xm2 - xm3 ) ;
  //
  const double k = std::sqrt ( Qm / Qp ) ;
  //
  std::cout 
    << " Qp= " << Qp
    << " Qm= " << Qm
    << " k = " << k 
    << std::endl ;
 
  //  if  ( 1 - k < 1.e-10 ) { return s_norm * p2 ; }  // RETURN 
  //
  const double sqrt_Qp  = std::sqrt ( Qp ) ;
  //
  const double sin_phi1 = sqrt_Qp / ( p2 + xm1 * xm1 - xm2 * xm2 - xm3 * xm3 ) ;
  const double sin_phi2 = sqrt_Qp / ( p2 - xm1 * xm1 + xm2 * xm2 - xm3 * xm3 ) ;
  const double sin_phi3 = sqrt_Qp / ( p2 - xm1 * xm1 - xm2 * xm2 + xm3 * xm3 ) ;
  //
  const double KZ1 = 
    Ostap::Math::elliptic_KZ ( std::asin ( sin_phi1 ) , k ) / ( sin_phi1 * sin_phi1 ) ;
  const double KZ2 = 
    Ostap::Math::elliptic_KZ ( std::asin ( sin_phi2 ) , k ) / ( sin_phi2 * sin_phi2 ) ;
  const double KZ3 = 
    Ostap::Math::elliptic_KZ ( std::asin ( sin_phi3 ) , k ) / ( sin_phi3 * sin_phi3 ) ;
  //
  const double E_minus_K = - Ostap::Math::elliptic_KmE ( k ) ;
  //
  return ( sqrt_Qp * ( p2 + xm1 * xm1 + xm2 * xm2 + xm3 * xm3 ) * E_minus_K
           + Qp * ( KZ1 + KZ2 + KZ3 ) ) / p2  * s_norm ;
}
// ============================================================================
//                                                                      The END 
// ============================================================================
