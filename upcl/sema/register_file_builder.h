#ifndef __upcl_sema_register_file_builder_h
#define __upcl_sema_register_file_builder_h

#include "sema/register_dep_tracker.h"
#include "c/register_def.h"
#include "c/register_set.h"

namespace upcl { namespace sema {

class register_file_builder {
	typedef std::map<std::string, c::register_def *> named_register_def_map;

	register_dep_tracker   *m_rdt;
	c::register_def_vector  m_rdefs;
	c::register_set_vector  m_rsets;
	named_register_def_map  m_named_rdefs;

public:
	register_file_builder();

	bool analyze(register_dep_tracker *rdt);

	inline c::register_def *get_register(std::string const &name) const
	{ 
		named_register_def_map::const_iterator i = m_named_rdefs.find(name);
		if (i != m_named_rdefs.end())
			return i->second;
		else
			return 0;
	}

	inline c::sub_register_def *get_register(std::string const &base,
			std::string const &name) const
	{ 
		if (name.empty())
			return 0;

		c::register_def *def = get_register(base);
		if (def == 0)
			return 0;
		else
			return def->get_sub_register(name);
	}

	inline c::register_def_vector const &get_physical_registers() const
	{ return m_rdefs; }

private:
	bool analyze_top(register_info const *);
	bool analyze_top(std::string const &, register_info_vector const &);

private:
	c::register_def *create_top(register_info const *ri);

private:
	c::register_def *create_sub(register_info const *, c::register_def *,
			register_info const *);
	
	c::register_def *create_aliased_sub(register_info const *,
			c::register_def *, c::type const *, register_info const *,
			register_info const *);
	c::register_def *create_pseudo_aliased_sub(register_info const *,
			c::register_def *, c::type *, register_info const *);

public:
	void dump_named() const;
};

} }

#endif  // !__upcl_sema_register_file_builder_h
