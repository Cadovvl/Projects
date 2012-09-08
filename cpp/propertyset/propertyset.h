
#ifndef __PROPERTYSET__
#define __PROPERTYSET__

#include <map>
#include <string>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

namespace cadovvl{
	//! Manipulator for internal 
	struct Property_manipulator_t{
		virtual ~Property_manipulator_t(){}
	};
	template<class ValueType>
	class Property_getter_manipulator_t : virtual public Property_manipulator_t{
		public:
		Property_getter_manipulator_t(boost::function< ValueType ( void ) > get_function) : getter(get_function) {} 
		typename boost::function< ValueType ( void ) > getter;
	};
	template<class ValueType>
	class Property_setter_manipulator_t : virtual public Property_manipulator_t{
		public:
		Property_setter_manipulator_t(boost::function< void ( ValueType ) > set_function) : setter(set_function) {}
		typename boost::function< void ( ValueType ) > setter;
	};
	template<class SetterValueType, class GetterValueType>
	class Property_inheritance_manipulator_t : public Property_setter_manipulator_t<SetterValueType>, public Property_getter_manipulator_t<GetterValueType> {
		public:
		Property_inheritance_manipulator_t(
			boost::function< void ( SetterValueType ) > set_function, 
			boost::function< GetterValueType ( void ) > get_function) : 
			Property_setter_manipulator_t<SetterValueType>(set_function), 
			Property_getter_manipulator_t<GetterValueType>(get_function) {}
	};
	
	//! Property set with abstract KeyType
	template<class KeyType>
	class	Property_set{
		typename std::map<KeyType, boost::shared_ptr< Property_manipulator_t> > _inheritance_properties;
		typename std::map<KeyType, std::string > _general_properties;
		protected:
		template <class GetterValueType, class SetterValueType>
		void add_property(KeyType key, 
			boost::function< void ( SetterValueType ) > set_function, 
			boost::function< GetterValueType ( void ) > get_function){
			_inheritance_properties[key] = boost::shared_ptr<Property_manipulator_t>( new Property_inheritance_manipulator_t<SetterValueType, GetterValueType>(set_function, get_function));
		}
		template <class SetterValueType>
		void add_property(KeyType key,
			boost::function< void ( SetterValueType ) > set_function){
			_inheritance_properties[key] = boost::shared_ptr<Property_manipulator_t>( new Property_setter_manipulator_t<SetterValueType>(set_function));
		}
		template <class GetterValueType> 
		void add_property(KeyType key, 
			boost::function< GetterValueType ( void ) > get_function){
			_inheritance_properties[key] = boost::shared_ptr<Property_manipulator_t>( new Property_getter_manipulator_t<GetterValueType>(get_function));
		}
		public:
		template <class ValueType>
		void set_property(KeyType key, ValueType value){
			typename std::map<KeyType, boost::shared_ptr< Property_manipulator_t > >::iterator manipulator = _inheritance_properties.find(key);
			if( manipulator != _inheritance_properties.end() ){
				Property_setter_manipulator_t<ValueType>* setter_ptr = 
					dynamic_cast<Property_setter_manipulator_t<ValueType>* >( manipulator->second.get() );
				if(setter_ptr){
					setter_ptr->setter(value);
					return;
				}
			}
			_general_properties[key] = boost::lexical_cast<std::string>(value);
		}
		template <class ValueType>
		ValueType get_property(KeyType key){
			typename std::map<KeyType, boost::shared_ptr< Property_manipulator_t > >::iterator manipulator = _inheritance_properties.find(key);
			if( manipulator != _inheritance_properties.end() ){
				Property_getter_manipulator_t<ValueType>* getter_ptr = 	
					dynamic_cast<Property_getter_manipulator_t<ValueType>*>( manipulator->second.get() );
				if(getter_ptr){
					return getter_ptr->getter();
				}
			}
			return boost::lexical_cast<ValueType>(_general_properties[key]);
		}
	};
	
	typedef Property_set<std::string> StringProperties;
	
};



#endif // __PROPERTYSET__

