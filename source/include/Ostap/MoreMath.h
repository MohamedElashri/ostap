// ============================================================================
#ifndef OSTAP_MOREMATH_H 
#define OSTAP_MOREMATH_H 1
// ============================================================================
// Include files
// ============================================================================
// STD& STL 
// ============================================================================
#include <vector>
#include <complex>
#include <cmath>
// ============================================================================
/** @file Ostap/MoreMath.h
 *  collection of various helper math functions  
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-26
 */  
// ============================================================================
namespace Ostap
{
  // ==========================================================================
  namespace Math 
  {
    // ========================================================================
    // some special functions 
    // ========================================================================
    /** sum of N-terms in the exponential expansion 
     *  \f[ f (x) = \sum_{n=0}^{N} \frac{x^k}{k!}\f]
     *  Abramowitz & Stegun, 6.5.11
     *  @param x  INPUT the argument 
     *  @param N  INPUT N-terms to be used 
     *  @return partial exponential sum 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-26
     */
    double exp_N ( const double x , const unsigned short N ) ;
    // ========================================================================
    /** "relative or reduced exponent"
     *  \f[ f(x) = N! ( e^{x} - \sum_{k=0}^{N-1} \frac{x^k}{k!})/x^{N} \f] 
     *  it also can be written as :
     *  \f[ f(x) =  =  \frac{e^x n!}{x^n} (1 - \Gamma(n,x)/\Gamma(n)) \f] 
     *  Abramowitz & Stegun, 4.2.41
     *  @param x  INPUT the argument 
     *  @param N  INPUT N-terms to be used 
     *  @return the value of "reduced exponent"
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-26
     */
    double exp_rel_N  ( const double x , const unsigned short N ) ;
    // ========================================================================
    /** compute \f$ f(x) = \frac{e^x-1}{x}\f$
     *  @return the value of the function 
     *  @see exp_rel_N 
     */        
    double exprel ( const double x ) ;
    // ========================================================================
    /** regularized incomplete gamma function 
     *  \f[ \gamma^{\ast}(a,x) = \frac{x^{-a}}{\Gamma(a)} \gamma(a,x) \f], 
     *  where \f[\gamma(a,x) = \int_0^x e^{-t}t^{a-1}dt\f], 
     *  Abramowitz & Stegun, 6.5.4
     *  @param a INPUT a-parameter 
     *  @param x INPUT x-argument 
     *  @return the value of regularized incomplete gamma function 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-27
     */
    double gamma_star ( const double a , const double x ) ;
    // ========================================================================
    /** regularized incomplete gamma function 
     *  \f[ \gamma^{\ast}(n,x) = \frac{x^{-n}}{\Gamma(n)} \gamma(n,x) \f], 
     *  where 
     *  \f[ \gamma(n,x) = \Gamma(n) - \Gamma(n,x) = \int_0^x e^{-t}t^{n-1}dt\f], 
     *  Abramowitz & Stegun, 6.5.4
     *  @param n INPUT n-parameter 
     *  @param x INPUT x-argument 
     *  @return the value of regularized incomplete gamma function 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-27
     */
    double gamma_star ( const int n , const double x ) ;
    // ========================================================================
    /** alpha_n 
     *  \f[\alpha_n(x) = \int_1^\infty t^n e^{-tx}dt\f] for \f$x>0\f$
     *  Abramowitz & Stegun, 5.1.5
     *  @param n INPUT n-parameter 
     *  @param x INPUT x-argument 
     *  @return the function value 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-27
     */
    double alpha_N ( const unsigned short n , const double x ) ;
    // ========================================================================
    /** complementary function to alpha_n 
     *  \f[\alpha^{\prime}_n(x) = \int_0^1 t^n e^{-tx}dt \f]
     *  @param n INPUT n-parameter 
     *  @param x INPUT x-argument 
     *  @return the function value 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-27
     */
    double alpha_prime_N ( const unsigned short n , const double x ) ;
    // ========================================================================
    /** beta_n 
     *  \f[\beta_n(x) = \int_{-1}^{+1} t^n e^{-tx}dt \f]
     *  Abramowitz & Stegun, 5.1.6
     *  @param n INPUT n-parameter 
     *  @param x INPUT x-argument 
     *  @return the function value 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-27
     */
    double beta_N ( const unsigned short n , const double x ) ;
    // ========================================================================
    /** confluent hypergeometrical function  1F1 aka Kummer's function
     *  \f[ f(a,b,x) = \sum_i  \frac{(a,i)}{((b,i)}\frac{x^i}{i!} \f] 
     *  @param a INPUT a-parameter 
     *  @param b INPUT b-argument  (b>0)
     *  @param x argument
     *  @return value of Kummer function
     */
    double kummer 
    ( const unsigned short a ,
      const unsigned short b , 
      const double         x ) ;
    // ========================================================================
    /** get quantile function for standard normal distribution
     *  @param alpha argument    <code> 0<alpha<1</code>  
     *  @see http://en.wikipedia.org/wiki/Probit
     *  @return quantile value 
     */
    double  probit ( const double alpha  ) ;
    // ========================================================================
    /** scaled complementary error function 
     *  \f[ 1 -  \mathrm{erf} (x) = e^{-x^2} \mathrm{erfcx}(x)  \f]
     *  @param x  the argument 
     *  @return the value of the scaled complementary error function 
     *  @attention  overflow happens for x<-26.6
     *  The actual implementation is copied from http://ab-initio.mit.edu/Faddeeva
     *  @see http://ab-initio.mit.edu/Faddeeva
     *  @see https://en.wikipedia.org/wiki/Error_function
     *  @see https://en.wikipedia.org/wiki/Faddeeva_function
     */
    double  erfcx ( const double x ) ;
    // ========================================================================
    /** complementary error function
     *  @see std::erfc 
     */
    inline  double  erfc  ( const double x ) { return std::erfc ( x ) ; }
    // ========================================================================
    /** error function
     *  @see std::erf 
     */
    inline  double  erf   ( const double x ) { return std::erf  ( x ) ; }
    // ========================================================================
    /** \overload complex error function (the error function of complex arguments)
     *  @param x  the argument 
     *  @return the value of the coplmex error function 
     *  The actual implementation is copied from http://ab-initio.mit.edu/Faddeeva
     *  @see http://ab-initio.mit.edu/Faddeeva
     *  @see https://en.wikipedia.org/wiki/Error_function
     *  @see https://en.wikipedia.org/wiki/Faddeeva_function
     */
    std::complex<double> erf   ( const std::complex<double>& x ) ;
    // ========================================================================
    /** \overload complementary complex error function 
     *  \f$ 1 -  erf (x) = erfc(x)  \f$         
     *  @param x  the argument 
     *  @return the value of the complementary complex error function 
     *  The actual implementation is copied from http://ab-initio.mit.edu/Faddeeva
     *  @see http://ab-initio.mit.edu/Faddeeva
     *  @see https://en.wikipedia.org/wiki/Error_function
     *  @see https://en.wikipedia.org/wiki/Faddeeva_function
     */
    std::complex<double> erfc  ( const std::complex<double>& x ) ;
    // ========================================================================
    /** scaled complementary error function for complex argument 
     *  \f[1 -  erf (x) = e^{-x^2} erfcx(x)  \f] 
     *  @param x  the argument 
     *  @return the value of the scaled complementary error function 
     *  The actual implementation is copied from http://ab-initio.mit.edu/Faddeeva
     *  @see http://ab-initio.mit.edu/Faddeeva
     *  @see https://en.wikipedia.org/wiki/Error_function
     *  @see https://en.wikipedia.org/wiki/Faddeeva_function
     */
    std::complex<double> erfcx ( const std::complex<double>& x ) ;
    // ========================================================================
    /** Imaginary error function 
     *  \f[\mathrm{erfi}(x) = -i \mathrm{erf}(ix) =\frac{2}{\sqrt{\pi}} \int_0^x e^{t^2}dt \f] 
     *  @param x the argument
     *  @return the value of the imaginary error function 
     *  The actual implementation is copied from http://ab-initio.mit.edu/Faddeeva
     *  @see http://ab-initio.mit.edu/Faddeeva
     *  @see https://en.wikipedia.org/wiki/Error_function
     */
    double               erfi ( const double x ) ;
    // ========================================================================
    /** \overload Imaginary error function 
     *  \f[\mathrm{erfi}(x) = -i \mathrm{erf}(ix) = \frac{2}{\sqrt{\pi}} \int_0^x e^{t^2}dt\f] 
     *  @param x the argument
     *  @return the value of the imaginary error function 
     *  The actual implementation is copied from http://ab-initio.mit.edu/Faddeeva
     *  @see http://ab-initio.mit.edu/Faddeeva
     *  @see https://en.wikipedia.org/wiki/Error_function
     */
    std::complex<double> erfi ( const std::complex<double>& x ) ;
    // ========================================================================
    /** compute Faddeeva "w" function:
     *  w(z) = exp(-z^2) erfc(-iz) [ Faddeeva / scaled complex error func ]
     *  @return the value of Faddeeva function 
     *  The actual implementation is copied from http://ab-initio.mit.edu/Faddeeva
     *  @see http://ab-initio.mit.edu/Faddeeva
     *  @see https://en.wikipedia.org/wiki/Error_function
     *  @see https://en.wikipedia.org/wiki/Faddeeva_function
     */
    std::complex<double> faddeeva_w ( const std::complex<double>& x ) ;
    // ========================================================================
    /** Dowson function 
     *  \f[ f(x) =  \frac{\sqrt{\pi}}{2}  *  e^{-z^2} * erfi(z) \f] 
     *  @return the value of Dawson function 
     *  The actual implementation is copied from http://ab-initio.mit.edu/Faddeeva
     *  @see http://ab-initio.mit.edu/Faddeeva
     *  @see https://en.wikipedia.org/wiki/Error_function
     *  @see https://en.wikipedia.org/wiki/Dowson_function
     */
    double               dowson     ( const double                x ) ;
    // ========================================================================
    /** \overload Compute Dowson function 
     *  \f[ f(x) =  \frac{\sqrt{\pi}}{2}  *  e^{-z^2} * erfi(z) \f] 
     *  @return the value of Dawson function 
     *  The actual implementation is copied from http://ab-initio.mit.edu/Faddeeva
     *  @see http://ab-initio.mit.edu/Faddeeva
     *  @see https://en.wikipedia.org/wiki/Error_function
     *  @see https://en.wikipedia.org/wiki/Dowson_function
     */
    std::complex<double> dowson     ( const std::complex<double>& x ) ;
    // ========================================================================
    /** Compute <code>sech</code> function 
     *  \f[ f(x) = \frac{1}{\cosh x} = \frac{2}{ e^{x}+e^{-x} }\f]
     *  @return the value of sech function 
     */
    double sech ( const double x ) ;
    // ========================================================================
    /** \overload Compute <code>sech</code> function 
     *  \f[ f(x) = \frac{1}{\cosh x} = \frac{2}{ e^{x}+e^{-x} }\f]
     *  @return the value of sech function 
     */
    std::complex<double> sech ( const std::complex<double>& x ) ;
    // ========================================================================
    /** Compute inverse Gamma function 
     *  \f[ f(x) = \frac{1}{\Gamma(x)} \f]
     *  @return the value of inverse Gamma functions 
     */
    double igamma ( const double x ) ;    
    // ========================================================================
    /** Compute psi function 
     *  \f[ f(x) = \frac{d}{dx}\ln \Gamma(x)\f]
     *  @return the value of psi function 
     */
    double psi ( const double x ) ;    
    // ========================================================================
    /** Pochhammer symbol, aka "rising factorial"
     *  \f[ P(x,n) = x ( x + 1) ( x + 1 ) ... ( x + n - 1 ) = \Pi^{k-1}_{k=0} (x + k) \f] 
     *  @see https://en.wikipedia.org/wiki/Falling_and_rising_factorials
     *  @see Ostap::Math::rising_factorial
     */
    double pochhammer
    ( const double         x , 
      const unsigned short n ) ;    
    // ========================================================================
    /** Rising  factorial, aka "Pochhammer's symbol"
     *  \f[ (x)^n = x ( x + 1) ( x + 1 ) ... ( x + n - 1 ) = \Pi^{k-1}_{k=0} (x + k) \f] 
     *  @see https://en.wikipedia.org/wiki/Falling_and_rising_factorials
     *  @see Ostap::Math::pochhammer 
     *  @see Ostap::Math::falling_factorial
     */
    double rising_factorial  ( const double x , const unsigned short n ) ;
    // =========================================================================
    /** Falling factorial
     *  \f[ (x)_n = \Pi^{k-1}_{k=0}  (x - k) \f] 
     *  @see https://en.wikipedia.org/wiki/Falling_and_rising_factorials
     *  @see Ostap::Math::rising_factorial
     */
    double falling_factorial ( const double x , const unsigned short n ) ;
    // =========================================================================
    /** Pochhammer symbol, aka "rising factorial" and its derivative 
     *  \f[ P(x,n) = x ( x + 1) ( x + 1 ) ... ( x + n - 1 ) = \Pi^{k-1}_{k=0} (x + k) \f] 
     *  @see https://en.wikipedia.org/wiki/Falling_and_rising_factorials
     *  @see Ostap::Math::rising_factorial
     *  @see Ostap::Math::pochhammer 
     */
    std::pair<double,double> pochhammer_with_derivative 
    ( const double         x , 
      const unsigned short n ) ;    
    // ========================================================================
    /** get the standard gaussian pdf 
     *  @see https://en.wikipedia.org/wiki/Normal_distribution
     *  @param x x-value  
     *  @param mu    mu (location)
     *  @param sigma sigma (width)  
     *  @return the value of gaussian pdf 
     */
    double gauss_pdf
    ( const double x         ,
      const double mu    = 0 ,
      const double sigma = 1 ) ;
    // ========================================================================
    /** get the standard gaussian cdf 
     *  @see https://en.wikipedia.org/wiki/Normal_distribution
     *  \f$ f(x) = \frac{1}{2} \left( 1 + erf ( \frac{x} { \sqrt{2} } ) \right) \f$ 
     *  @param x x-value  
     *  @param mu    mu (location)
     *  @param sigma sigma (width)  
     *  @return the value of gaussian cdf 
     */
    double gauss_cdf 
    ( const double x         ,
      const double mu    = 0 ,
      const double sigma = 1 ) ;
    // ========================================================================
    /** get the Gaussian integral 
     *  @see https://en.wikipedia.org/wiki/Normal_distribution
     *  \f$ f(x) = \frac{1}{2} \left( 1 + erf ( \frac{x} { \sqrt{2} } ) \right) \f$ 
     *  \f[ f(a,b;\mu,\sigma = \int_a^b \frac{1}{\sqrt{2\pi}\sigma}
     *     \mathrm{e}^{-\frac{1}{2} \left( \frac{x-\mu}{\sigma}\right)^2}dx \f]
     *  @param a low integration limit
     *  @param b high integration limit
     *  @param mu location of Gaussian
     *  @param sigm awidth of the Gaussian
     */
    double gauss_int
    ( const double a         ,
      const double b         ,
      const double mu    = 0 ,
      const double sigma = 1 ) ;
    // ========================================================================
    /** Student's t-CDF 
     *  \f[ f(t;\nu) = \left\{
     *  \begin{array}{ll}
     *  1-\frac{1}{2}I_{x(t}}\left(\frac{\nu}{2}, \frac{1}{2}\right)   
     *   & \mathrm{for}~t\ge0 \                                     \
     *  \frac{1}{2}I_{x(t}}\left(\frac{\nu}{2}, \frac{1}{2}\right)   
     *   & \mathrm{for}~t\<0
     *  \end{array} \right. f]
     *  where \f$ x(t) = \frac{\nu}{t^2+\nu}\f$ and 
     *  \f$I_{x}(a,b)\f$ is incomplete beta function; 
     *  @param  t t-value 
     *  @param  nu parameter nu , $\nu>0$
     */
    double student_cdf ( const double t  , 
                         const double nu ) ;
    // ========================================================================
    /** compute Owen's T-function
     *  \f$ f(h,a) = \frac{1}{2\pi}\int_0^a \frac{ e^{ -\frac{1}{2} h^2(1+x^2)}}{1+x^2}dx \f$ 
     *  @see https://en.wikipedia.org/wiki/Owen%27s_T_function
     *  @see http://people.sc.fsu.edu/~burkardt/f_src/owens/owens.html
     *  @param h  h-parameter
     *  @param a  a-parameter
     *  @return the value of Owen's T-function
     *  The actual code is copied from http://people.sc.fsu.edu/~burkardt/f_src/owens/owens.html
     */
    double owen  ( const double h , const double a ) ;
    // ========================================================================
    /** get the gaussian integral
     *  \f[ f = \int_a^b \exp { -\alpha^2 x^2 + \beta x } \mathrm{d}x \f]
     *  @param alpha the alpha parameter
     *  @param beta  the beta  parameter
     *  @param low   the low  integration limit
     *  @param high  the high integration limit
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2010-05-23
     */
    double gaussian_integral
    ( const double alpha ,
      const double beta  ,
      const double low   ,
      const double high  ) ;
    // ========================================================================
    /** get the gaussian integral
     *  \f[ f = \int_{a}^{_\infty} \exp { -\alpha^2 x^2 + \beta x } \mathrm{d}x \f]
     *  @param alpha the alpha parameter
     *  @param beta  the beta  parameter
     *  @param low   the low  integration limit
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2010-05-23
     */
    double gaussian_integral_right
    ( const double alpha ,
      const double beta  ,
      const double low   ) ;
    // ========================================================================
    /** get the gaussian integral
     *  \f[ f = \int_{-\infty}^b \exp { -\alpha^2 x^2 + \beta x } \mathrm{d}x \f]
     *  @param alpha the alpha parameter
     *  @param beta  the beta  parameter
     *  @param high  the high integration limit
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2010-05-23
     */
    double gaussian_integral_left
    ( const double alpha ,
      const double beta  ,
      const double high  ) ;
    // ========================================================================

    // ========================================================================
    // Elliptic integrals 
    // ========================================================================
    /** Trigonometric form of incomplete elliptic integral \f$ F(\phi,k) \f$
     *  \f[ F(\phi,k) \equiv \int_{0}^{\phi} \dfrac{ d \psi }{\sqrt{1-k^2 \sin^2 \phi }}\f] 
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     */
    double elliptic_F 
    ( const double phi , 
      const double k   ) ;
    // ========================================================================
    /** Trigonometric form of incomplete elliptic integral \f$ E(\phi,k) \f$
     *  \f[ F(\phi,k) \equiv \int_{0}^{\phi} \sqrt{1-k^2 \sin^2 \phi } d \psi \f] 
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     */
    double elliptic_E 
    ( const double phi , 
      const double k   ) ;
    // ========================================================================
    /** Complete elliptic integral \f$ E(k) \f$  
     *  \[ E(k) \equiv E ( \frac{\pi}{2}, k ) \f] 
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     */
    double elliptic_E 
    ( const double k   ) ;
    // ========================================================================
    /** Complete elliptic integral \f$ K(k) \f$  
     *  \[ K(k) \equiv F ( \frac{\pi}{2}, k ) \f] 
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     */
    double elliptic_K 
    ( const double k   ) ;
    // ========================================================================
    /** difference in complete elliptic integrals  \f$ K(k) \f$ and \f$ E(k) \f$
     *  \f[ K(k) - E(k) = \frac{k^2}{3}R_D\left(0,1-k^2,1\right)\f],
     *  where \f$ R_D(x,y,z)\f$ is a symmetric Carlson form 
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double elliptic_KmE 
    ( const double k   ) ;    
    // ========================================================================
    /** Jacobi zeta function
     *  \f[ K(k) Z( \beta , k ) = K(k) E(\beta, k ) - E(k) F(\beta,k) \f] 
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  http://functions.wolfram.com/EllipticIntegrals/JacobiZeta/introductions/IncompleteEllipticIntegrals/ShowAll.html
     */
    double elliptic_Z  
    ( const double beta , 
      const double k    ) ;
    // ========================================================================
    /** Product of Jacobi zeta function \f$ Z(\beta,k) \f$
     *  and complete elliptic integral \f$ K(k) \f$
     *  \f[ K(k) Z( \beta , k ) = \frac{k^2}{3} \sin \beta \cos \beta 
     *   \sqrt{ 1 - k^2\sin^2\beta } R_J\left(0,1-k^2, 1 , 1-k^2\sin^2\beta\right)\f], 
     *  where \f$ R_J(x,y,z,t)\f$ is a symmetric Carlson form  
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double elliptic_KZ 
    ( const double beta  , 
      const double k   ) ;
    // ========================================================================
    /** elliptic \f$ \Pi(\alpha^2,k)\f$ function 
     *  - \f$ alpha^2 < 1 \f$ 
     *  - \f$ k      < 1 \f$ 
     *  \f[ \Pi(\alpha^2, k) - K(k) = 
     *   \frac{1}{3}\alpha^2 R_J( 0, 1-k^2, 1 , 1 - \alpha^2) \f] 
     */ 
    double elliptic_PI
    ( const double alpha2 , 
      const double k      ) ;
    // ========================================================================
    /** elliptic \f$ \Pi(\alpha^2,k) - K(k) \f$ function 
     *  \f[ \Pi(\alpha^2, k) - K(k) \equiv  
     *   \frac{1}{3}\alpha^2 R_J( 0, 1-k^2, 1 , 1 - \alpha^2) \f] 
     *  - \f$ alpha^2 < 1 \f$ 
     *  - \f$ k      < 1 \f$ 
     */ 
    double elliptic_PImK  
    ( const double alpha2 , 
      const double k      ) ;
    // ========================================================================

    // ========================================================================
    // Symmetric Carlson forms 
    // ========================================================================
    
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_F(x,y,z) = \int_0^{+\infty} 
     *   \left[  (t+x)(t+y)(t+z) \right]^{-1/2}dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RF 
    ( const double x , 
      const double y , 
      const double z ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_F(x,y,z) = \int_0^{+\infty} 
     *   \left[  (t+x)(t+y)(t+z) \right]^{-1/2}dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RF_gsl  
    ( const double x , 
      const double y , 
      const double z ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_F(x,y,z) = \int_0^{+\infty} 
     *   \left[  (t+x)(t+y)(t+z) \right]^{-1/2}dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RF_int 
    ( const double x , 
      const double y , 
      const double z ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_J(x,y,z,p) = \int_0^{+\infty} 
     *   \left[  (t+x)(t+y)(t+z) \right]^{-1/2} (t+p) dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RJ
    ( const double x , 
      const double y , 
      const double z , 
      const double p ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_J(x,y,z,p) = \int_0^{+\infty} 
     *   \left[  (t+x)(t+y)(t+z) \right]^{-1/2} (t+p) dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RJ_gsl
    ( const double x , 
      const double y , 
      const double z , 
      const double p ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_J(x,y,z,p) = \int_0^{+\infty} 
     *   \left[  (t+x)(t+y)(t+z) \right]^{-1/2} (t+p) dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RJ_int
    ( const double x , 
      const double y , 
      const double z , 
      const double p ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_C(x,y) = R_F(x,y,y) = \int_0^{+\infty} 
     *   (t+x)^{-1/2}(t+y)^{-1}dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     *  For negative y, Cauchy principal value it returned 
     *  \f[ R_C(x,-y) = \left(\frac{x}{x+y}\right)R_C(x+y,y), 0 < y \f] 
     */
    double carlson_RC
    ( const double x , 
      const double y ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_C(x,y) = R_F(x,y,y) = \int_0^{+\infty} 
     *   (t+x)^{-1/2}(t+y)^{-1}dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     *  For negative y, Cauchy principal value it returned 
     *  \f[ R_C(x,-y) = \left(\frac{x}{x+y}\right)R_C(x+y,y), 0 < y \f] 
     */
    double carlson_RC_gsl 
    ( const double x , 
      const double y ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_C(x,y) = R_F(x,y,y) = \int_0^{+\infty} 
     *   (t+x)^{-1/2}(t+y)^{-1}dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     *  For negative y, Cauchy principal value it returned 
     *  \f[ R_C(x,-y) = \left(\frac{x}{x+y}\right)R_C(x+y,y), 0 < y \f] 
     */
    double carlson_RC_int
    ( const double x , 
      const double y ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_D (x,y,z) = R_J(x,y,z,z) = \int_0^{+\infty} 
     *  \left[  (t+x)(t+y)\right]^{-1/2} (t+z)^{-3/2} dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RD 
    ( const double x , 
      const double y , 
      const double z ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_D (x,y,z) = R_J(x,y,z,z) = \int_0^{+\infty} 
     *  \left[  (t+x)(t+y)\right]^{-1/2} (t+z)^{-3/2} dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RD_gsl  
    ( const double x , 
      const double y , 
      const double z ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_D (x,y,z) = R_J(x,y,z,z) = \int_0^{+\infty} 
     *  \left[  (t+x)(t+y)\right]^{-1/2} (t+z)^{-3/2} dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RD_int  
    ( const double x , 
      const double y , 
      const double z ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_G (x,y,z) = \rac{1}{4} \int_0^{+\infty} 
     *  \left[  (t+x)(t+y)\right]^{-1/2} 
     *   \left( \frac{x}{t+x} + \frac{y}{t+y} + \frac{z}{t+z}\right)  dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RG 
    ( const double x , 
      const double y , 
      const double z ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_G (x,y,z) = \rac{1}{4} \int_0^{+\infty} 
     *  \left[  (t+x)(t+y)\right]^{-1/2} 
     *   \left( \frac{x}{t+x} + \frac{y}{t+y} + \frac{z}{t+z}\right)  dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RG_gsl 
    ( const double x , 
      const double y , 
      const double z ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_G (x,y,z) = \rac{1}{4} \int_0^{+\infty} 
     *  \left[  (t+x)(t+y)\right]^{-1/2} 
     *   \left( \frac{x}{t+x} + \frac{y}{t+y} + \frac{z}{t+z}\right)  dt \f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RG_int 
    ( const double x , 
      const double y , 
      const double z ) ;
    // ========================================================================
    // specific cases of symmetric forms 
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_F(x,y) = R_F(x,y,0)\f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RF
    ( const double x , 
      const double y ) ;
    // ========================================================================
    /** Symmetric Carlson form 
     *  \f[ R_G(x,y) = R_G(x,y,0)\f]
     *  @see https://en.wikipedia.org/wiki/Elliptic_integral
     *  @see Carlson, B.C., "Numerical computation of real or complex elliptic integrals", 
     *                Numerical Algorithms, 10, 1995,  13
     *  @see https://doi.org/10.1007/BF02198293
     *  @see https://arxiv.org/abs/math/9409227
     */
    double carlson_RG 
    ( const double x , 
      const double y ) ;
    // ========================================================================

    // ========================================================================
    /** Helpful function \f$ H_a(a,u_1,u_2)\f$ for the relativistic Voigt profile
     * 
     * The relativistic Voigt profile \f$ V_2(m;\mu,\Gamma,\sigma) \f$  is
     *  \f$ V_2(m; \mu,\Gamma,\sigma) \equiv  S_2(m;\mu,\Gamma)\ast G(\delta m;\sigma)\f$ 
     *  where 
     *  - \f$ S_2 = \frac{1}{\pi}\frac{\mu\Gamma}{ (m^2-\mu^2)^2 + \mu^2\Gamma^2 } \f$    
     *  - \f$ G(\delta m ; \sigma) = \frac{1}{\sqrt{2\pi\sigma^2}} 
     *     \mathrm{e}^{-\frac{1}{2} \left( \frac{\delta m }{\sigma} \right)^2} \f$$
     *  
     *  \f$ V_2(m; \mu,\Gamma,\sigma = \frac{H_2(a,u_1,u_2)}{2\sqrt{\pi}\sigma^2}\f$, where 
     *  - \f$ u_1 = \frac{m-\mu }{\sqrt{2}\sigma} \f$
     *  - \f$ u_2 = \frac{m+\mu }{\sqrt{2}\sigma} \f$
     *  - \f$ a   = \frac{\mu\Gamma}{2\sigma^2} \f$
     *
     *  \f[ H_2(a,u_1,u_2) = 
     *   \frac{a}{\pi} \int_{-\infty}{+\infty}  
     *    \frac{dt}{  (u_1-t)^2(u_2-t0^2 + a^2 } \f] 
     * @see Kycia, Radoslaw A.; Jadach, Stanislaw. 
     *      "Relativistic Voigt profile for unstable particles in high energy physics". 
     *      Journal of Mathematical Analysis and Applications. 463 (2): 1040–1051 
     *      arXiv:1711.09304. doi:10.1016/j.jmaa.2018.03.065. ISSN 0022-247X.
     * @see https://arxiv.org/abs/1711.09304
     */
    double H2 ( const double a  , 
                const double u1 , 
                const double u2 ) ;
    
    
    // ========================================================================
    /** modified Bessel function of the second kind  
     *  \f$ K_n(x) \f$ for \f$ x>0 \f$
     *  @see https://en.wikipedia.org/wiki/Bessel_function#Modified_Bessel_functions_:_I%CE%B1,_K%CE%B1
     *  @see gsl_sf_bessel_K0_e 
     *  @see gsl_sf_bessel_K1_e 
     *  @see gsl_sf_bessel_Kn_e 
     */
    double bessel_Kn         ( const int   n  , const double x ) ;
    // ========================================================================
    /** scaled modified Bessel function of the second kind 
     *  \f$ \mathrm{e}^x K_n(x) \f$ for \f$ x>0 \f$
     *  @see https://en.wikipedia.org/wiki/Bessel_function#Modified_Bessel_functions_:_I%CE%B1,_K%CE%B1
     *  @see gsl_sf_bessel_K0_scaled_e 
     *  @see gsl_sf_bessel_K1_scaled_e 
     *  @see gsl_sf_bessel_Kn_scaled_e 
     */
    double bessel_Kn_scaled  ( const int   n  , const double ) ;
    // ========================================================================
    /** modified Bessel function of the second kind  
     *  \f$ K_{\nu}(x) \f$ for \f$ x>0, \nu>0 \f$
     *  @see https://en.wikipedia.org/wiki/Bessel_function#Modified_Bessel_functions_:_I%CE%B1,_K%CE%B1
     *  @see gsl_sf_bessel_Knu_e 
     */
    double bessel_Knu        ( const double nu , const double x ) ;
    // ========================================================================
    /** scaled modified Bessel function of the second kind 
     *  \f$ \mathrm{e}^x K_{\nu}(x) \f$ for \f$ x>0, \nu>0 \f$
     *  @see https://en.wikipedia.org/wiki/Bessel_function#Modified_Bessel_functions_:_I%CE%B1,_K%CE%B1
     *  @see gsl_sf_bessel_Knu_scaled_e 
     */
    double bessel_Knu_scaled ( const double  nu , const double x ) ;
    
    
    
    // ========================================================================
    // clenshaw summation algorithms 
    // ========================================================================
    
    // ========================================================================
    /** Clenshaw algorithm for summation of Chebyshev polynomials 
     *  \f$ f(x) = \sum_i p_i T_i(x)\f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double clenshaw_chebyshev 
    ( const std::vector<double>& pars , 
      const double               x    ) ;
    // =========================================================================
    /** Clenshaw algorithm for summation of Legendre polynomials 
     *  \f$ f(x) = \sum_i p_i P_i(x) \f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double clenshaw_legendre
    ( const std::vector<double>& pars , 
      const double               x    ) ;
    // ========================================================================
    /** Clenshaw algorithm for summation of monomial series 
     *  (aka Horner rule) 
     *  \f$ f(x) = \sum_i a_i x^i \f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double clenshaw_polynom
    ( const std::vector<double>& pars , 
      const double               x    ) ;
    // ========================================================================
    /** Clenshaw algorithm for summation of monomial series (aka Horner rule) 
     *  \f$ f(x) = \sum_i a_i x^i \f$, such as \f$f(0)= a_0\f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double horner_a0
    ( const std::vector<double>& pars , 
      const double               x    ) ;
    // ========================================================================
    /** Clenshaw algorithm for summation of monomial series (aka Horner rule) 
     *  \f$ f(x) = \sum_i a_i x^{n-i} \f$, such as \f$f(0)= a_n\f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double horner_aN 
    ( const std::vector<double>& pars , 
      const double               x    ) ;
    // ========================================================================
    /** Clenshaw algorithm for summation of cosine-series 
     *  \f$ f(x) = \frac{a_0}{2} + \sum_{i=k}^{n} a_k \cos( k x) \f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double clenshaw_cosine
    ( const std::vector<double>& pars , 
      const double               x    ) ;
    // ========================================================================
    /** Clenshaw algorithm for summation of sine-series 
     *  \f$ f(x) = \sum_{i=k}^{n} a_k \sin( k x) \f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double clenshaw_sine
    ( const std::vector<double>& pars , 
      const double               x    ) ;
    // ========================================================================
    /** Clenshaw algorithm for summation of Fourier-series 
     *  \f$ f(x) = \frac{a_0}{2} + \sum_{i=k}^{n} a_{2k-1}\sin(kx)+a_{2k}\cos(kx) \f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double clenshaw_fourier 
    ( const std::vector<double>& pars , 
      const double               x    ) ;
    // ========================================================================
    /** Clenshaw algorithm for summation of Hermite polynomials 
     *  \f$ f(x) = \sum_i p_i He_i(x) \f$
     *  @attention here we consider "probabilistic" polynomials
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double clenshaw_hermite
    ( const std::vector<double>& pars , 
      const double               x    ) ;
    // =========================================================================

    // ========================================================================
    // continued fractions 
    // ========================================================================
    
    // ========================================================================
    /** evaluate "simple" continued fraction 
     *  \f$f(x) = a_0 + \frac{1}{ a_1 + \frac{1}{ a_2 + ...} } \f$
     *  @param a  INPUT  coefficients  
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double continued_fraction_simple 
    ( const std::vector<double>& a ) ;
    // ========================================================================
    /** evaluate "simple" continued fraction 
     *  \f$f(x) = \frac{b_0}{ 1 + \frac{b_1}{ 1 + ...}} \f$
     *  @param b  INPUT  coefficients  
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double continued_fraction_b
    ( const std::vector<double>& b ) ;
    // ========================================================================
    /** evaluate the continued fraction 
     *  \f$f(x) = [b_0+]  \frac{a_1}{ b_1 + \frac{a_2}{ b_2 + ...}} \f$
     *  @param a  INPUT  coefficients, (length = N  )
     *  @param b  INPUT  coefficients, (length = N or N+1)
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-02-10
     */
    double continued_fraction
    ( const std::vector<double>& a , 
      const std::vector<double>& b ) ;
    // ========================================================================    

    // ========================================================================    
    /** get the intermediate polynom \f$ g_l (x)\f$ used for the calculation of 
     *  the angular-momentum Blatt-Weisskopf centrifugal-barrier factor 
     *  @see S.U.Chung "Formulas for Angular-Momentum Barrier Factors", BNL-QGS-06-01
     *  @see https://physique.cuso.ch/fileadmin/physique/document/2015_chung_brfactor1.pdf
     *
     *  The complex-valued polynomials \f$ g_l(x) \f$  with integer 
     *  coefficients can be written as 
     *  \f[ g_l(x) = \sum_{k=0}^{l} a_{lk}(-ix)^{l-k}\f] with
     *  \f$ a_{lk} = \frac{(l+k)!}{2^k k! (l-k)!}\f$ and \f$a_{l0}=1\f$.
     *
     *  It satisfies the recurrent relation
     *  \f[ g_{l+1}(x) = (2l+1)g_l(x) -  x^2 g_{l-1}(x)\f] 
     *  with the initial values of \f$ g_0(x) \equiv 1 \f$ 
     *  and \f$ g_1(x) \equiv -ix + 1\f$.
     *  This recurrense relation is used for the actual calculation.
     *
     *  @param  x  the value of scaled relative momentum 
     *  @param  l  the orbital momentum 
     *  @return the value of \f$ g_l(x) \f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2019-10-13
     *  @see Ostap::Math::barrier_factor 
     *  @see Ostap::Math::barrier_absg 
     */
    std::complex<double>
    barrier_g 
    ( const double       x ,
      const unsigned int l ) ;
    // ========================================================================
    /** get the angular-momentum Blatt-Weisskopf centrifugal-barrier factor 
     *  @see S.U.Chung "Formulas for Angular-Momentum Barrier Factors", BNL-QGS-06-01
     *  @see https://physique.cuso.ch/fileadmin/physique/document/2015_chung_brfactor1.pdf
     *  the fuction evaluates 
     *  \f[ f_l(a) \equiv \frac{1}{\left| x h_l^{(1)}(x)\right|} \f], 
     *  where \f$ h_l^{(1)}\f$ is a spherical Hankel  function of the first kind.
     *  Actually \f$ f_k(x)\f$ is calculated as 
     *  \f[ f_l(a) x^l \left| g_l(x) \right|^{-1} \f], 
     *  where \f$ g_l(x)\f$ is a complex-valued polynomial with integer coefficients, 
     *  that satisfies the recurrent relation
     *  \f[ g_{l+1}(x) = (2l+1)g_l(x) -  x^2 g_{l-1}(x)\f] 
     *  with the initial values of \f$ g_0(x) \equiv 1 \f$ 
     *  and \f$ g_1(x) \equiv -ix + 1\f$ 
     *   
     *  \f$  \left. f_l(x) \right|_{x\rightarrow 0}  = \mathcal{O}(x^l) \f$,
     *  \f$  \left. f_l(x) \right|_{x\rightarrow +\infty}= 1  \f$,
     *
     *  @param  x  the value of scaled relative momentum 
     *  @param  l  the orbital momentum 
     *  @return the value of the angular-momentum Blatt-Weisskopf centrifugal-barrier factor
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2019-10-13                       
     *  @see Ostap::Math::barrier_g
     *  @see Ostap::Math::barrier_absg
     */
    inline double barrier_factor ( const double x , const unsigned int l ) 
    { return std::pow  ( x , l ) / std::abs ( barrier_g ( x , l ) ) ; }
    // ========================================================================    
    /** get the absolute value of the intermediate polynom 
     *  \f$ \left| g_l (x) \rigth| \f$ used for the calculation of 
     *  the angular-momentum Blatt-Weisskopf centrifugal-barrier factor 
     *  @see S.U.Chung "Formulas for Angular-Momentum Barrier Factors", BNL-QGS-06-01
     *  @see https://physique.cuso.ch/fileadmin/physique/document/2015_chung_brfactor1.pdf
     *  The complex-valued polynomials \f$ g_l(x) \f$  with integer coefficients 
     *  satisfies the recurrent relation
     *  \f[ g_{l+1}(x) = (2l+1)g_l(x) -  x^2 g_{l-1}(x)\f] 
     *  with the initial values of \f$ g_0(x) \equiv 1 \f$ 
     *  and \f$ g_1(x) \equiv -ix + 1\f$ 
     *  @param  x  the value of scaled relative momentum 
     *  @param  l  the orbital momentum 
     *  @return the absolute value of \f$ g_l(x) \f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2019-10-13
     *  @see Ostap::Math::barrier_factor 
     *  @see Ostap::Math::barrier_g
     */
    inline double barrier_absg ( const double x , const unsigned int l ) 
    { return std::abs ( barrier_g ( x , l ) ) ; }
    // ========================================================================
    
    // ========================================================================
  } //                                             end of namespace Ostap::Math 
  // ==========================================================================
} //                                                     end of namespace Ostap 
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // OSTAP_MOREMATH_H
// ============================================================================
