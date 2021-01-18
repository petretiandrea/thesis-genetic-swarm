//
// Created by andrea on 04/01/21.
//

#ifndef SWARM_GEN_JSONPRETTIFY_H
#define SWARM_GEN_JSONPRETTIFY_H

#include <boost/json.hpp>

namespace boost {
    namespace json {
        void pretty_print( std::ostream& os, boost::json::value const& jv, std::string* indent = nullptr)
        {
            namespace json = boost::json;
            std::string indent_;
            if(! indent)
                indent = &indent_;
            switch(jv.kind())
            {
                case json::kind::object:
                {
                    os << "{\n";
                    indent->append(4, ' ');
                    auto const& obj = jv.get_object();
                    if(! obj.empty())
                    {
                        auto it = obj.begin();
                        for(;;)
                        {
                            os << *indent << json::serialize(it->key()) << " : ";
                            pretty_print(os, it->value(), indent);
                            if(++it == obj.end())
                                break;
                            os << ",\n";
                        }
                    }
                    os << "\n";
                    indent->resize(indent->size() - 4);
                    os << *indent << "}";
                    break;
                }

                case json::kind::array:
                {
                    os << "[\n";
                    indent->append(4, ' ');
                    auto const& arr = jv.get_array();
                    if(! arr.empty())
                    {
                        auto it = arr.begin();
                        for(;;)
                        {
                            os << *indent;
                            pretty_print( os, *it, indent);
                            if(++it == arr.end())
                                break;
                            os << ",\n";
                        }
                    }
                    os << "\n";
                    indent->resize(indent->size() - 4);
                    os << *indent << "]";
                    break;
                }

                case json::kind::string:
                {
                    os << json::serialize(jv.get_string());
                    break;
                }

                case json::kind::uint64:
                    os << jv.get_uint64();
                    break;

                case json::kind::int64:
                    os << jv.get_int64();
                    break;

                case json::kind::double_:
                    os << jv.get_double();
                    break;

                case json::kind::bool_:
                    if(jv.get_bool())
                        os << "true";
                    else
                        os << "false";
                    break;

                case json::kind::null:
                    os << "null";
                    break;
            }

            if(indent->empty())
                os << "\n";
        }
    }
}

#endif //SWARM_GEN_JSONPRETTIFY_H
