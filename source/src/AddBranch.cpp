// ============================================================================
// Include files 
// ============================================================================
#include <string>
#include <tuple>
// ============================================================================
// ROOT
// ============================================================================
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
// ============================================================================
// Ostap
// ============================================================================
#include "Ostap/StatusCode.h"
#include "Ostap/AddBranch.h"
#include "Ostap/Funcs.h"
#include "Ostap/Notifier.h"
// ============================================================================
/** @file
 *  Implementation file for function Ostap::Trees::add_branch 
 *  @see Ostap::Trees::add_branch 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2019-05-14
 */
// ============================================================================
namespace 
{
  // ==========================================================================
  enum {
    INVALID_TREE          = 750 , 
    CANNOT_CREATE_BRANCH  = 751 , 
    CANNOT_CREATE_FORMULA = 752 , 
    INVALID_TREEFUNCTION  = 753 , 
    INVALID_TH2           = 754 , 
    INVALID_TH1           = 755 , 
    INVALID_BUFFER        = 756 , 
  };
  // ==========================================================================
}
// ============================================================================
/* add new branch with name <code>name</code> to the tree
 * the value of the branch is taken from  function <code>func</code>
 * @param tree    input tree 
 * @param name    the name for new branch 
 * @param func    the function to be used to fill the branch 
 * @return status code 
 * @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 * @date 2019-05-14
 */
// ============================================================================
Ostap::StatusCode 
Ostap::Trees::add_branch 
( TTree*                  tree ,  
  const std::string&      name , 
  const Ostap::IFuncTree& func ) 
{
  if ( !tree ) { return Ostap::StatusCode ( INVALID_TREE ) ; }
  //
  Double_t value    = 0  ;
  TBranch* branch   = tree->Branch( name.c_str() , &value , (name + "/D").c_str() );
  if ( !branch ) { return Ostap::StatusCode ( CANNOT_CREATE_BRANCH ) ; }
  //
  const TObject* o = dynamic_cast<const TObject*>( &func ) ;
  //
  Ostap::Utils::Notifier notifier { tree , o ? const_cast<TObject*>(o) : nullptr } ;
  //
  // due to  some strange reasons we need to invoke the Notifier explicitely.
  // - otherwise FuncTH1::Notify causes crash..
  notifier.Notify() ;
  //
  const Long64_t nentries = tree->GetEntries(); 
  for ( Long64_t i = 0 ; i < nentries ; ++i )
  {
    if ( tree->GetEntry ( i ) < 0 ) { break ; };
    //
    value  =  func ( tree  ) ;
    //
    branch -> Fill (       ) ;
  }
  //
  return Ostap::StatusCode::SUCCESS ;  
}
// =============================================================================
/*   add new branch with name <code>name</code> to the tree
 *   the value of the branch is taken from the function <code>func</code>
 *   @param tree    input tree 
 *   @param name    the name for new branch 
 *   @param formula the fomula use to calculate new  branch
 *   @return status code 
 *   @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *   @date 2019-05-14
 */
// =============================================================================
Ostap::StatusCode 
Ostap::Trees::add_branch 
( TTree*             tree    ,  
  const std::string& name    , 
  const std::string& formula ) 
{
  if ( !tree ) { return Ostap::StatusCode ( INVALID_TREE ) ; }
  //
  auto func = std::make_unique<Ostap::Functions::FuncFormula>( formula ,  tree ) ;
  if ( !func ) { return Ostap::StatusCode ( CANNOT_CREATE_FORMULA ) ; }
  //
  Ostap::Utils::Notifier notifier ( tree , func.get () ) ;
  //
  return add_branch ( tree , name , *func ) ;
}
// ============================================================================
/*  add new branches to the tree
 *  the value of the branch each  is taken from <code>branches</code>
 *  @param tree     input tree 
 *  @param name     the name for new branch 
 *  @param branches the map name->formula use to calculate newbranch
 *  @return status code 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2019-05-14
 */
// ============================================================================
Ostap::StatusCode 
Ostap::Trees::add_branch 
( TTree*                                   tree     ,  
  const std::map<std::string,std::string>& branches ) 
{
  //
  if      ( !tree            ) { return Ostap::StatusCode ( INVALID_TREE ) ; }
  else if ( branches.empty() ) { return Ostap::StatusCode::SUCCESS         ; }
  //
  typedef FUNCTREEMAP                                                  MAP   ;
  typedef std::vector<std::unique_ptr<Ostap::Functions::FuncFormula> > STORE ;
  //
  STORE store ;
  MAP   map ;
  //
  store.reserve( branches.size () ) ;
  //
  Ostap::Utils::Notifier notifier ( tree ) ;
  //
  for ( const auto& entry : branches )
  {
    store.emplace_back ( std::make_unique<Ostap::Functions::FuncFormula>( entry.second ,  tree ) ) ;
    auto const& func = store.back() ;
    if ( !func ) { return Ostap::StatusCode ( CANNOT_CREATE_FORMULA ) ; }
    //
    notifier.add ( func.get() )   ;
    map [ entry.first ] = func.get () ;    
    //
  }
  //
  // due to some very strange reasons we need to invoke the Notifier explicitely.
  // - otherwise crash could happen causes crash..
  notifier.Notify() ;
  //
  return add_branch ( tree , map ) ;
}
// ============================================================================
/*  add new branches to the tree
 *  the value of the branch each  is taken from <code>branches</code>
 *  @param tree     input tree 
 *  @param name     the name for new branch 
 *  @param branches the map name->function use to calculate new branch
 *  @return status code 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2019-05-14
 */
// ============================================================================
Ostap::StatusCode 
Ostap::Trees::add_branch 
( TTree*                           tree     ,  
  const Ostap::Trees::FUNCTREEMAP& branches ) 
{
  // 
  if      ( !tree            ) { return Ostap::StatusCode ( INVALID_TREE ) ; }
  else if ( branches.empty() ) { return Ostap::StatusCode::SUCCESS         ; }
  //
  typedef std::vector<const Ostap::IFuncTree*> FUNCTIONS ;
  typedef std::vector<double>                  VALUES    ;
  typedef std::vector<TBranch*>                BRANCHES  ;
  //
  const std::size_t N = branches.size() ;
  VALUES    values    ( N ) ;
  BRANCHES  tbranches ( N ) ;
  FUNCTIONS functions ( N ) ;
  //
  // Notifier
  Ostap::Utils::Notifier notifier { tree } ;
  //
  unsigned int index = 0 ;
  for ( const auto& entry : branches ) 
  {
    const auto&             name = entry.first  ;
    const Ostap::IFuncTree* func = entry.second ;
    if ( !func   ) { return Ostap::StatusCode ( INVALID_TREEFUNCTION ) ; }
    functions [ index ] = func ;
    //
    const TObject* o = dynamic_cast<const TObject*>( func ) ;
    if ( nullptr != o ) { notifier.add ( const_cast<TObject*> ( o ) ) ; }
    //
    TBranch* branch   = tree->Branch
      ( name.c_str() ,
        &values[index] , (name + "/D").c_str() );
    if ( !branch ) { return Ostap::StatusCode ( CANNOT_CREATE_BRANCH ) ; }
    tbranches [ index ] = branch ;
    //
    ++index ;
  }
  //
  // due to some very strange reasons we need to invoke the Notifier explicitely.
  // - otherwise crash could happen  
  //
  notifier.Notify() ;
  //
  const Long64_t nentries = tree->GetEntries(); 
  for ( Long64_t i = 0 ; i < nentries ; ++i )
  {
    if ( tree->GetEntry ( i ) < 0 ) { break ; };
    //
    // evaluate the functions
    for ( unsigned int k = 0 ; k < N ; ++k ) { values    [ k ] = (*functions[ k ]) ( tree ) ; }
    // fill the branches
    for ( unsigned int j = 0 ; j < N ; ++j ) { tbranches [ j ] -> Fill ()                   ; }
  }
  //
  return Ostap::StatusCode::SUCCESS ; 
}
// ============================================================================
/*  add new branch to TTree, sampling it from   the 1D-histogram
 *  @param tree (UPFATE) input tree 
 *  @param name   name of the new branch 
 *  @param histo  the historgam to be  sampled
 *  @return status code 
 *  @see TH1::GetRandom 
 */
// ============================================================================
Ostap::StatusCode 
Ostap::Trees::add_branch 
( TTree*               tree  , 
  const std::string&   name  , 
  const TH1&           histo )
{
  if ( !tree ) { return Ostap::StatusCode ( INVALID_TREE ) ; }
  //
  const TH1* h1 = &histo ;
  if ( nullptr != dynamic_cast<const TH2*>( h1 ) ) 
  { return Ostap::StatusCode ( INVALID_TH1 ) ; }
  //
  Double_t value    = 0  ;
  TBranch* branch   = tree->Branch( name.c_str() , &value , (name + "/D").c_str() );
  if ( !branch ) { return Ostap::StatusCode ( CANNOT_CREATE_BRANCH ) ; }
  //
  const Long64_t nentries = tree->GetEntries(); 
  for ( Long64_t i = 0 ; i < nentries ; ++i )
  {
    if ( tree->GetEntry ( i ) < 0 ) { break ; };
    //
    value = histo.GetRandom() ;
    //
    branch -> Fill (       ) ;
  }
  //
  return Ostap::StatusCode::SUCCESS ;  
}
// ============================================================================
/** add new branch to TTree, sampling it from   the 1D-histogram
 *  @param tree (UPFATE) input tree 
 *  @param namex  name of the new branch 
 *  @param namey  name of the new branch 
 *  @param histo  the historgam to be  sampled
 *  @return status code 
 *  @see TH2::GetRandom2 
 */
// ============================================================================
Ostap::StatusCode 
Ostap::Trees::add_branch 
( TTree*               tree  , 
  const std::string&   namex , 
  const std::string&   namey , 
  const TH2&           histo )
{
  if ( !tree ) { return Ostap::StatusCode ( INVALID_TREE ) ; }
  //
  const TH2* h2 = &histo ;
  if ( nullptr != dynamic_cast<const TH3*>( h2 ) ) 
  { return Ostap::StatusCode ( INVALID_TH2 ) ; }
  //
  Double_t value_x   = 0  ;
  TBranch* branch_x  = tree->Branch( namex.c_str() , &value_x , (namex + "/D").c_str() );
  if ( !branch_x ) { return Ostap::StatusCode ( CANNOT_CREATE_BRANCH ) ; }
  //
  Double_t value_y   = 0  ;
  TBranch* branch_y  = tree->Branch( namey.c_str() , &value_y , (namey + "/D").c_str() );
  if ( !branch_y ) { return Ostap::StatusCode ( CANNOT_CREATE_BRANCH ) ; }
  //
  TH2& h = const_cast<TH2&> ( histo ) ;
  //
  const Long64_t nentries = tree->GetEntries(); 
  for ( Long64_t i = 0 ; i < nentries ; ++i )
  {
    if ( tree->GetEntry ( i ) < 0 ) { break ; };
    //
    h.GetRandom2 ( value_x , value_y ) ;
    //
    branch_x -> Fill (       ) ;
    branch_y -> Fill (       ) ;
  }
  //
  return Ostap::StatusCode::SUCCESS ;  
}
// ============================================================================
/** add new branch to TTree, sampling it from   the 1D-histogram
 *  @param tree (UPFATE) input tree 
 *  @param namex  name of the new branch 
 *  @param namey  name of the new branch 
 *  @param namez  name of the new branch 
 *  @param histo  the historgam to be  sampled
 *  @return status code 
 *  @see TH2::GetRandom2 
 */
// ============================================================================
Ostap::StatusCode 
Ostap::Trees::add_branch 
( TTree*               tree  , 
  const std::string&   namex , 
  const std::string&   namey , 
  const std::string&   namez , 
  const TH3&           histo )
{
  if ( !tree ) { return Ostap::StatusCode ( INVALID_TREE ) ; }
  //
  Double_t value_x   = 0  ;
  TBranch* branch_x  = tree->Branch( namex.c_str() , &value_x , (namex + "/D").c_str() );
  if ( !branch_x ) { return Ostap::StatusCode ( CANNOT_CREATE_BRANCH ) ; }
  //
  Double_t value_y   = 0  ;
  TBranch* branch_y  = tree->Branch( namey.c_str() , &value_y , (namey + "/D").c_str() );
  if ( !branch_y ) { return Ostap::StatusCode ( CANNOT_CREATE_BRANCH ) ; }
  //
  Double_t value_z   = 0  ;
  TBranch* branch_z  = tree->Branch( namez.c_str() , &value_z , (namez + "/D").c_str() );
  if ( !branch_z ) { return Ostap::StatusCode ( CANNOT_CREATE_BRANCH ) ; }
  //
  TH3& h = const_cast<TH3&> ( histo ) ;
  //
  const Long64_t nentries = tree->GetEntries(); 
  for ( Long64_t i = 0 ; i < nentries ; ++i )
  {
    if ( tree->GetEntry ( i ) < 0 ) { break ; };
    //
    h.GetRandom3 ( value_x , value_y , value_z ) ;
    //
    branch_x -> Fill (       ) ;
    branch_y -> Fill (       ) ;
    branch_z -> Fill (       ) ;
  }
  //
  return Ostap::StatusCode::SUCCESS ;  
}
// ============================================================================
namespace 
{
  // ==========================================================================
  template <class DATA>
  inline Ostap::StatusCode 
  _add_branch_ ( TTree*               tree  , 
                 const std::string&   vname ,                
                 const std::string&   vtype , 
                 const DATA*          data  ,
                 const unsigned long  size  , 
                 const DATA           value ) 
  {
    //
    if ( !tree ) { return Ostap::StatusCode ( INVALID_TREE   ) ; }
    if ( !data ) { return Ostap::StatusCode ( INVALID_BUFFER ) ; }
    //    
    DATA  bvalue  { value } ;
    TBranch* branch = tree->Branch( vname.c_str() , &bvalue , ( vname + vtype ).c_str() );
    //
    const Long64_t total    = tree->GetEntries() ;
    const Long64_t nentries = total < size ? total : size ;
    //
    for ( Long64_t i = 0 ; i < nentries ; ++i ) 
    {
      bvalue =  *(data + i) ;
      branch->Fill() ;
    }
    //
    for ( Long64_t i = nentries ; i < total  ; ++i ) 
    {
      bvalue = value ;
      branch->Fill() ;
    }
    //
    return Ostap::StatusCode::SUCCESS ;  
  }
  // ==========================================================================
}
// ============================================================================
#if ROOT_VERSION(6,24,0)<=ROOT_VERSION_CODE
// ============================================================================
/*  copy data from buffer into new branch 
 *  @param tree   The tree 
 *  @param data   input data fuffer 
 *  @param size   length of the buffer
 *  @param value  default value (used for short buffers) 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*               tree  , 
  const std::string&   vname ,  
  const double*        data  , 
  const unsigned long  size  , 
  const double         value ) 
{ return _add_branch_ ( tree , vname , "/D" , data , size , value ) ; }
// ============================================================================
/*  copy data from buffer into new branch 
 *  @param tree   The tree 
 *  @param data   input data fuffer 
 *  @param size   length of the buffer
 *  @param value  default value (used for short buffers) 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*               tree  , 
  const std::string&   vname ,  
  const float*         data  , 
  const unsigned long  size  , 
  const float          value ) 
{ return _add_branch_ ( tree , vname , "/F" , data , size , value ) ; }
// ========================================================================
/*  copy data from buffer into new branch 
 *  @param tree   The tree 
 *  @param data   input data fuffer 
 *  @param size   length of the buffer
 *  @param value  default value (used for short buffers) 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*               tree  , 
  const std::string&   vname ,  
  const short*         data  , 
  const unsigned long  size  , 
  const short          value ) 
{ return _add_branch_ ( tree , vname , "/S" , data , size , value ) ; }
// ============================================================================
/*  copy data from buffer into new branch 
 *  @param tree   The tree 
 *  @param data   input data fuffer 
 *  @param size   length of the buffer
 *  @param value  default value (used for short buffers) 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*                tree  , 
  const std::string&    vname ,  
  const unsigned short* data  , 
  const unsigned long   size  , 
  const unsigned short  value ) 
{ return _add_branch_ ( tree , vname , "/s" , data , size , value ) ; }
// ============================================================================
/*  copy data from buffer into new branch 
 *  @param tree   The tree 
 *  @param data   input data fuffer 
 *  @param size   length of the buffer
 *  @param value  default value (used for short buffers) 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*               tree  , 
  const std::string&   vname ,  
  const int*           data  , 
  const unsigned long  size  , 
  const int            value ) 
{ return _add_branch_ ( tree , vname , "/I" , data , size , value ) ; }
// ============================================================================
/*  copy data from buffer into new branch 
 *  @param tree   The tree 
 *  @param data   input data fuffer 
 *  @param size   length of the buffer
 *  @param value  default value (used for short buffers) 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*               tree  , 
  const std::string&   vname ,  
  const unsigned int*  data  , 
  const unsigned long  size  , 
  const unsigned int   value ) 
{ return _add_branch_ ( tree , vname , "/i" , data , size , value ) ; }
// ============================================================================
/*  copy data from buffer into new branch 
 *  @param tree   The tree 
 *  @param data   input data fuffer 
 *  @param size   length of the buffer
 *  @param value  default value (used for short buffers) 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*               tree  ,
    const std::string&   vname ,
    const long*          data  , 
    const unsigned long  size  , 
    const long           value ) 
{ return _add_branch_ ( tree , vname , "/L" , data , size , value ) ; }
// ============================================================================
/*  copy data from buffer into new branch 
 *  @param tree   The tree 
 *  @param data   input data fuffer 
 *  @param size   length of the buffer
 *  @param value  default value (used for short buffers) 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*               tree  , 
    const std::string&   vname ,  
    const unsigned long* data  , 
    const unsigned long  size  , 
    const unsigned long value ) 
{ return _add_branch_ ( tree , vname , "/l" , data , size , value ) ; }
// ============================================================================
#endif
// ============================================================================
/** copy data from buffer into new branch 
 *  @param tree    The tree 
 *  @param namex   name of the new branch 
 *  @param value   the value 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*               tree        , 
  const std::string&   vname       ,  
  const double         value       ) 
{ return _add_branch_ ( tree , vname , "/D" , &value , 1 , value ) ; }
// ============================================================================
/** copy data from buffer into new branch 
 *  @param tree    The tree 
 *  @param namex   name of the new branch 
 *  @param value   the value 
 *  @return status code 
 */
// ============================================================================
Ostap::StatusCode
Ostap::Trees::add_branch 
( TTree*               tree        , 
  const std::string&   vname       ,  
  const int            value       ) 
{ return _add_branch_ ( tree , vname , "/I" , &value , 1 , value ) ; }



// ============================================================================
//                                                                      The END 
// ============================================================================
