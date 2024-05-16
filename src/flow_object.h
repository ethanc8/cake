/*
   Object represents "memory" and state. Used by flow analysis
*/

#pragma once
#include "ownership.h"
#include "type.h"


struct flow_visit_ctx;

extern unsigned int s_visit_number; //creates a unique number

enum object_state
{
    /*
       Not applicable. The state cannot be used.
       struct...
       TODO we need empty state when object does not exist
       and the merge it is not a problem
       merge with nothing then
    */
    OBJECT_STATE_NOT_APPLICABLE = 0,

    OBJECT_STATE_UNINITIALIZED = 1 << 0,

    OBJECT_STATE_NULL = 1 << 1,
    OBJECT_STATE_NOT_NULL = 1 << 2,

    //means not-null moved at same time
    OBJECT_STATE_MOVED = 1 << 3,

    OBJECT_STATE_ZERO = 1 << 5,
    OBJECT_STATE_NOT_ZERO = 1 << 6,

    OBJECT_STATE_LIFE_TIME_ENDED = 1 << 7,
};

bool is_moved(enum object_state e);
bool is_not_null(enum object_state e);
bool is_null(enum object_state e);

bool is_not_zero(enum object_state e);
bool is_zero(enum object_state e);

bool maybe_is_null(enum object_state e);
bool is_uninitialized(enum object_state e);

void object_state_to_string(enum object_state e);

struct objects {
    struct object* owner* owner data;
    int size;
    int capacity;
};

void objects_clear(struct objects* p);
void objects_destroy(struct objects* obj_owner p);
int objects_push_back(struct objects* p, struct object* owner p_object);
const struct object* objects_find(const struct objects* p, const struct object* p_object);


struct objects_view {
    struct object** owner opt data;
    int size;
    int capacity;
};

void objects_view_destroy(struct objects_view* obj_owner p);
int objects_view_push_back(struct objects_view* p, struct object* p_object);
bool objects_view_find(const struct objects_view* p, const struct object* p_object);
void objects_view_copy(struct objects_view* dest, const struct objects_view* source);
void objects_view_merge(struct objects_view* dest, const struct objects_view* source);
void objects_view_clear(struct objects_view* p);



struct object_state_stack_item {
    const char* name;
    int state_number;
    enum object_state state;
    struct objects_view ref;
};

struct object_state_stack
{
    struct object_state_stack_item* owner data;
    int size;
    int capacity;
};
void object_state_stack_destroy(struct object_state_stack* obj_owner p);
int object_state_stack_push_back(struct object_state_stack* p, enum object_state e, struct objects_view* pointed_ref, const char* name, int state_number);


/*
  Used in flow analysis to represent the object instance
*/
struct object
{
    //used to avoid infinite recursion
    unsigned int visit_number;

    enum object_state state;
    struct objects_view pointed;

    /*object are the result of expressions or they are declarators*/
    const struct declarator* p_declarator_origin;
    const struct expression* p_expression_origin;

    struct objects members;
    struct object_state_stack object_state_stack;

    int id; //helps debugging
    bool is_temporary;
};

void object_set_pointer(struct object* p_object, struct object* p_object2);

void object_destroy(struct object* obj_owner p);
void object_delete(struct object* owner opt p);
void object_swap(struct object* a, struct object* b);

struct declarator;
struct object* make_object(struct flow_visit_ctx* ctx,
                                 struct type* p_type,
                            const struct declarator* p_declarator_opt,
                            const struct expression* p_expression_origin);

void object_add_copy_current_state(struct object* object, const char* name, int state_number);
void object_add_empty_state(struct object* object, const char* name, int state_number);
struct token* object_get_token(const struct object* object);
void object_remove_state(struct object* object, int state_number);
int object_merge_current_state_with_state_number(struct object* object, int state_number);
void object_merge_current_state_with_state_number_or(struct object* object, int state_number);
int object_restore_current_state_from(struct object* object, int state_number);
void object_set_state_from_current(struct object* object, int state_number);
void object_merge_state(struct object* pdest, struct object* object1, struct object* object2);


struct flow_visit_ctx;
struct token;

void visit_object(struct flow_visit_ctx* ctx,
    struct type* p_type,
    struct object* p_object,
    const struct token* position_token,
    const char* previous_names,
    bool is_assigment);

void object_restore_state(struct object* object, int state_to_restore);


void print_object(struct type* p_type, struct object* p_object, bool short_version);

void set_direct_state(
    struct type* p_type,
    struct object* p_object,
    enum object_state flags);

void set_object(
    struct type* p_type,
    struct object* p_object,
    enum object_state flags);



void flow_assignment(struct flow_visit_ctx* ctx,
    const struct token* error_position,
    enum  assigment_type assigment_type,
    bool check_uninitialized_b,
    bool a_type_is_view,
    bool a_type_is_nullable,
    struct type* p_a_type, struct object* p_a_object,
    struct type* p_b_type, struct object* p_b_object);

void object_set_unknown(struct type* p_type, bool t_is_nullable, struct object* p_object, bool nullable_enabled);
void object_set_zero(struct type* p_type, struct object* p_object);
void object_set_uninitialized(struct type* p_type, struct object* p_object);
void object_set_nothing(struct type* p_type, struct object* p_object);
void object_set_moved(struct type* p_type, struct object* p_object);
void object_set_pointed_to_nothing(struct type* p_type, struct object* p_object);

void checked_read_object(struct flow_visit_ctx* ctx,
    struct type* p_type,
    bool is_nullable,
    struct object* p_object,
    const struct token* position_token,
    bool check_pointed_object);

bool object_is_zero_or_null(const struct object* p_object);
void end_of_storage_visit(struct flow_visit_ctx* ctx,
    struct type* p_type,
    bool type_is_view,
    struct object* p_object,
    const struct token* position_token,
    const char* previous_names);


bool object_is_expansible(const struct object* p_object);
void expand_pointer_object(struct flow_visit_ctx* ctx, struct type* p_type, struct object* p_object);
void object_push_states_from(const struct object* p_object_from, struct object* p_object_to);

struct object* expression_get_object(struct flow_visit_ctx* ctx, struct expression* p_expression, bool nullable_enabled);