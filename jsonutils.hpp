#ifndef DEEPNET_JSONUTILS
#define DEEPNET_JSONUTILS

#undef max
#undef min

#include <stdexcept>
#include <string>
#include <vector>
#include <array>

#include "json.h"


class json_exception : public std::exception {
public:
	json_exception(char* what) : std::exception(what) {}
	json_exception(const char* what) : std::exception(what) {}
};

class json_bad_input_exception : public json_exception {
public:
	json_bad_input_exception(char* what) : json_exception(what) {}
	json_bad_input_exception(const char* what) : json_exception(what) {}
};

class json_bad_type_exception : public json_exception {
public:
	json_bad_type_exception(char* what) : json_exception(what) {}
	json_bad_type_exception(const char* what) : json_exception(what) {}
};

class json_bad_length_exception : public json_exception {
public:
	json_bad_length_exception(char* what) : json_exception(what) {}
	json_bad_length_exception(const char* what) : json_exception(what) {}
};

class json_wrong_name_exception : public json_exception {
public:
	json_wrong_name_exception(char* what) : json_exception(what) {}
	json_wrong_name_exception(const char* what) : json_exception(what) {}
};

struct json_parser {
private:
	json_value_s* m_rootmem;
	json_object_s* m_root;
	json_object_element_s* m_current;
	bool m_owner;

public:
	json_parser() : m_root(nullptr), m_current(nullptr), m_owner(false), m_rootmem(nullptr) {
	}
	~json_parser() {
		if (m_rootmem != nullptr && m_owner) {
			free(m_rootmem);
		}
		m_root = nullptr;
		m_current = nullptr;
		m_owner = false;
		m_rootmem = nullptr;
	}

	json_parser(const json_parser&) = delete; //json_parse owns memory
	json_parser& operator=(const json_parser&) = delete;

	json_parser(json_parser&& other) noexcept : m_rootmem(other.m_rootmem), m_root(other.m_root), m_current(other.m_current), m_owner(other.m_owner) {
		other.~json_parser();
	}

	json_parser(json_object_s* root) noexcept : m_rootmem(nullptr), m_root(root), m_current(root->start), m_owner(true) {

	}

	json_parser(const std::string& body) noexcept {
		m_rootmem = json_parse(body.c_str(), body.size());
		if (m_rootmem == nullptr) {
			throw json_bad_input_exception("Could not parse the given input");
		}
		m_root = json_value_as_object(m_rootmem);
		m_current = m_root->start;
		m_owner = true;
	}

	template <class T>
	bool validate(json_object_element_s* elem) {
		if (elem->value != nullptr && elem->value->type != json_type_null && elem->value->payload != nullptr) {
			return true;
		}
		else
		{
			throw json_exception("Failed to validate json object element");
		}
	}

	template <class T>
	bool validate(json_array_element_s* elem) {
		if (m_root == nullptr) {
			throw json_exception("Attempt to use uninitialized json_parser");
		}

		if (elem->value != nullptr && elem->value->type != json_type_null && elem->value->payload != nullptr) {
			return true;
		}
		else
		{
			throw json_exception("Failed to validate json array element");
		}
	}

	template<>
	bool validate<bool>(json_object_element_s* elem) {
		if (elem->value != nullptr && elem->value->type != json_type_null && (elem->value->type == json_type_true || elem->value->type == json_type_false)) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}

	template<>
	bool validate<bool>(json_array_element_s* elem) {
		if (elem->value != nullptr && elem->value->type != json_type_null && (elem->value->type == json_type_true || elem->value->type == json_type_false)) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}

	template<>
	bool validate<int>(json_object_element_s* elem) {
		if (validate<void>(elem) && elem->value->type == json_type_number) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}

	template<>
	bool validate<int>(json_array_element_s* elem) {
		if (validate<void>(elem) && elem->value->type == json_type_number) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}

	template<>
	bool validate<long>(json_object_element_s* elem) {
		return validate<int>(elem);
	}

	template<>
	bool validate<unsigned long>(json_object_element_s* elem) {
		return validate<int>(elem);
	}

	template<>
	bool validate<long long>(json_object_element_s* elem) {
		return validate<int>(elem);
	}

	template<>
	bool validate<unsigned long long>(json_object_element_s* elem) {
		return validate<int>(elem);
	}

	template <>
	bool validate<float>(json_object_element_s* elem) {
		return validate<int>(elem);
	}

	template <>
	bool validate<double>(json_object_element_s* elem) {
		return validate<int>(elem);
	}

	template<>
	bool validate<std::string>(json_object_element_s* elem) {
		if (validate<void>(elem) && elem->value->type == json_type_string) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}

	template<>
	bool validate<long>(json_array_element_s* elem) {
		return validate<int>(elem);
	}

	template<>
	bool validate<unsigned long>(json_array_element_s* elem) {
		return validate<int>(elem);
	}

	template<>
	bool validate<long long>(json_array_element_s* elem) {
		return validate<int>(elem);
	}

	template<>
	bool validate<unsigned long long>(json_array_element_s* elem) {
		return validate<int>(elem);
	}

	template <>
	bool validate<float>(json_array_element_s* elem) {
		return validate<int>(elem);
	}

	template <>
	bool validate<double>(json_array_element_s* elem) {
		return validate<int>(elem);
	}

	template<>
	bool validate<std::string>(json_array_element_s* elem) {
		if (validate<void>(elem) && elem->value->type == json_type_string) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}

	template<std::array>
	bool validate(json_object_element_s* elem) {
		if (validate<void>(elem) && elem->value->type == json_type_array) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}

	template<std::array>
	bool validate(json_array_element_s* elem) {
		if (validate<void>(elem) && elem->value->type == json_type_array) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}

	template <std::vector>
	bool validate(json_object_element_s* elem) {
		if (validate<void>(elem) && elem->value->type == json_type_array) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}

	template <std::vector>
	bool validate(json_array_element_s* elem) {
		if (validate<void>(elem) && elem->value->type == json_type_array) {
			return true;
		}
		else
		{
			throw json_bad_type_exception("Type mismatch");
		}
	}



	template <class T>
	bool named_validate(json_object_element_s* elem, const std::string_view& name) {
		if (elem != nullptr && name.size() == elem->name->string_size && strcmp(name.data(), elem->name->string) == 0 && validate<T>(elem)) {
			return true;
		}
		else
		{
			//validate() will throw validate fail if it's an invalid elem, we just concern with throwing the name mismatch case
			throw json_wrong_name_exception("Element name mismatch");
		}
	}



	template <class T>
	json_parser& get(T& out) {
		//static_assert(false, "Unsupported type");
	}

	template <class T>
	json_parser& get(T& out, json_array_element_s* elem) {
		//static_assert(false, "Unsupported type");
	}

	template <>
	json_parser& get(bool& out) {
		if (validate<bool>(m_current)) {
			out = m_current->value->type;
			m_current = m_current->next;
		}
		return *this;
	}

	template <>
	json_parser& get(bool& out, json_array_element_s* elem) {
		if (validate<bool>(elem)) {
			out = elem->value->type;
		}
		return *this;
	}

	template <>
	json_parser& get(int& out) {
		if (validate<int>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			out = std::stoi(x->number);
			m_current = m_current->next;
		}
		return *this;
	}

	template <>
	json_parser& get(int& out, json_array_element_s* elem) {
		if (validate<int>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			out = std::stoi(x->number);
		}
		return *this;
	}


	template<>
	json_parser& get(int8_t& out) {
		if (validate<int>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			auto res = std::stoi(x->number);
			if (res >= std::numeric_limits<int8_t>::max()) {
				throw std::domain_error("parsed value above out type capacity");
			}
			else if(res <= std::numeric_limits<int8_t>::min()) {
				throw std::domain_error("parsed value below out type range");
			}
			out = res;
			m_current = m_current->next;
		}
		return *this;
	}

	template<>
	json_parser& get(int8_t& out, json_array_element_s* elem) {
		if (validate<int>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			auto res = std::stoi(x->number);
			if (res >= std::numeric_limits<int8_t>::max()) {
				throw std::domain_error("parsed value above out type capacity");
			}
			else if (res <= std::numeric_limits<int8_t>::min()) {
				throw std::domain_error("parsed value below out type range");
			}
			out = res;
		}
		return *this;
	}

	template<>
	json_parser& get(uint8_t& out) {
		if (validate<int>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			auto res = std::stoi(x->number);

			if (res <= std::numeric_limits<uint8_t>::min()) {
				throw std::domain_error("parsed value below out type range");
			}
			out = res;
			m_current = m_current->next;
		}
		return *this;
	}

	template<>
	json_parser& get(uint8_t& out, json_array_element_s* elem) {
		if (validate<int>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			auto res = std::stoi(x->number);
			
			if (res <= std::numeric_limits<uint8_t>::min()) {
				throw std::domain_error("parsed value below out type range");
			}
			out = res;
		}
		return *this;
	}

	template<>
	json_parser& get(int16_t& out) {
		if (validate<int>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			auto res = std::stoi(x->number);
			if (res >= std::numeric_limits<int16_t>::max()) {
				throw std::domain_error("parsed value above out type capacity");
			}
			else if (res <= std::numeric_limits<int16_t>::min()) {
				throw std::domain_error("parsed value below out type range");
			}
			out = res;
			m_current = m_current->next;
		}
		return *this;
	}

	template<>
	json_parser& get(int16_t& out, json_array_element_s* elem) {
		if (validate<int>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			auto res = std::stoi(x->number);
			if (res >= std::numeric_limits<int16_t>::max()) {
				throw std::domain_error("parsed value above out type capacity");
			}
			else if (res <= std::numeric_limits<int16_t>::min()) {
				throw std::domain_error("parsed value below out type range");
			}
			out = res;
		}
		return *this;
	}

	template<>
	json_parser& get(uint16_t& out) {
		if (validate<int>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			auto res = std::stoi(x->number);

			if (res <= std::numeric_limits<uint16_t>::min()) {
				throw std::domain_error("parsed value below out type range");
			}
			out = res;
			m_current = m_current->next;
		}
		return *this;
	}

	template<>
	json_parser& get(uint16_t& out, json_array_element_s* elem) {
		if (validate<int>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			auto res = std::stoi(x->number);

			if (res <= std::numeric_limits<uint16_t>::min()) {
				throw std::domain_error("parsed value below out type range");
			}
			out = res;
		}
		return *this;
	}

	template<>
	json_parser& get(long& out) {
		if (validate<long>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			out = std::stol(x->number);
			m_current = m_current->next;
		}
		return *this;
	}

	template <>
	json_parser& get(long& out, json_array_element_s* elem) {
		if (validate<long>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			out = std::stol(x->number);
		}
		return *this;
	}

	template<>
	json_parser& get(long long& out) {
		if (validate<long long>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			out = std::stoll(x->number);
			m_current = m_current->next;
		}
		return *this;
	}

	template<>
	json_parser& get(long long& out, json_array_element_s* elem) {
		if (validate<long long>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			out = std::stoll(x->number);
		}
		return *this;
	}

	template<>
	json_parser& get(unsigned long& out) {
		if (validate<int>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			out = std::stoul(x->number);
			m_current = m_current->next;
		}
		return *this;
	}

	template<>
	json_parser& get(unsigned long& out, json_array_element_s* elem) {
		if (validate<int>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			out = std::stoul(x->number);
		}
		return *this;
	}

	template<>
	json_parser& get(unsigned long long& out) {
		if (validate<int>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			out = std::stoull(x->number);
			m_current = m_current->next;
		}
		return *this;
	}

	template<>
	json_parser& get(unsigned long long& out, json_array_element_s* elem) {
		if (validate<int>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			out = std::stoull(x->number);
		}
		return *this;
	}

	template <>
	json_parser& get(float& out) {
		if (validate<int>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			out = std::stof(x->number);
			m_current = m_current->next;
		}
		return *this;
	}

	template <>
	json_parser& get(float& out, json_array_element_s* elem) {
		if (validate<int>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			out = std::stof(x->number);
		}
		return *this;
	}

	template <>
	json_parser& get(double& out) {
		if (validate<int>(m_current)) {
			auto x = (json_number_s*)(m_current->value->payload);
			out = std::stod(x->number);
			m_current = m_current->next;
		}
		return *this;
	}

	template <>
	json_parser& get(double& out, json_array_element_s* elem) {
		if (validate<int>(elem)) {
			auto x = (json_number_s*)(elem->value->payload);
			out = std::stod(x->number);
		}
		return *this;
	}

	template <>
	json_parser& get(std::string& out) {
		if (validate<std::string>(m_current)) {
			auto x = (json_string_s*)(m_current->value->payload);
			out = x->string;
			m_current = m_current->next;
		}
		return *this;
	}

	template <>
	json_parser& get(std::string& out, json_array_element_s* elem) {
		if (validate<std::string>(elem)) {
			auto x = (json_string_s*)(elem->value->payload);
			out = x->string;
		}
		return *this;
	}

	template<class T, size_t N>
	json_parser& get(std::array<T, N>& out) {
		if (validate<std::array<T, N>>(m_current)) {

			auto x = (json_array_s*)(m_current->value->payload);

			if (x->length != N) {
				throw json_bad_length_exception("Length mismatch");
			}

			json_array_element_s* current = x->start;
			for (int i = 0; i < N; i++) {
				get<T>(out[i], current);
				current = current->next;
			}

			m_current = m_current->next;
		}
		return *this;
	}

	template<class T, size_t N>
	json_parser& get(std::array<T, N>& out, json_array_element_s* elem) {
		if (validate<std::array<T,N>>(elem) < std::array<T, N>(elem)) {
			auto x = (json_array_s*)(elem->value->payload);

			if (x->length != N) {
				throw json_bad_length_exception("Length mismatch");
			}

			json_array_element_s* current = x->start;
			for (int i = 0; i < N; i++) {
				get<T>(out[i], current);
				current = current->next;
			}
		}
		return *this;
	}

	template <class T>
	json_parser& get(std::vector<T>& out) {
		if (validate<std::vector<T>>(m_current)) {
			auto x = (json_array_s*)(m_current->value->payload);

			out.resize(x->length);

			json_array_element_s* current = x->start;
			for (auto& entry : out) {
				get(entry, current);
				current = current->next;
			}
			m_current = m_current->next;
		}
		return *this;
	}

	template <class T>
	json_parser& get(std::vector<T>& out, json_array_element_s* elem) {
		if (validate<std::vector<T>>(elem)) {
			auto x = (json_array_s*)(elem->value->payload);

			out.resize(x->length);

			json_array_element_s* current = x->start;
			for (auto& entry : out) {
				get(entry, current);
				current = current->next;
			}
		}
		return *this;
	}



	template <class T>
	json_parser& get_named(T& out, const std::string_view& name) {
		if (named_validate<T>(m_current, name)) {
			get<T>(out);
		}
		return *this;
	}

	template <class T>
	json_parser& get_named_optional(T& out, const std::string_view& name) {
		try {
			if (named_validate<T>(m_current, name)) {
				get<T>(out);
			}
		}
		catch (json_wrong_name_exception& e) {
			//continue
		}
		return *this;
	}
};


#endif //ifndef header guard
