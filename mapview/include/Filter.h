#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
using namespace boost;
using namespace boost::numeric;
// using namespace boost::numeric::ublas;

extern void loadExpData		( const char * filename,
				  std::vector< string > & title,
				  ublas::matrix< double > & entire );

extern void saveBinaryData	( const char * filename,
				  const std::vector< string > & title,
				  const ublas::matrix< double > & entire );
extern void loadBinaryData	( const char * filename,
				  std::vector< string > & title,
				  ublas::matrix< double > & entire );

extern void loadEntireData	( const char * filename, ublas::matrix< double > & entire );
extern void downSample		( ublas::matrix< double > & entire,
				  ublas::matrix< double > & subset,
				  unsigned int stepSize );
extern void limitSample		( ublas::matrix< double > & entire,
				  ublas::matrix< double > & region,
				  const double cx, const double cy );
