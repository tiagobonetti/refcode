#include <cstdint>
#include <cassert>

// Obfuscated type
struct obf_t;

// Contructor Destructor
obf_t* build_obf(uint32_t value);
void destroy_obf(obf_t* obf);

// Compare Functions
bool equal_obf(obf_t* l, obf_t* r);
int compare_obf(obf_t* l, obf_t* r);

// Hash
std::size_t hash_obf(const obf_t* obf);

// Type definition
struct obf_t {
    uint32_t value;
};

obf_t* build_obf(uint32_t value)
{
    obf_t* obf = new obf_t;
    obf->value = value;
    return obf;
}

void destroy_obf(obf_t* obf)
{
    delete obf;
}

bool equal_obf(obf_t* l, obf_t* r)
{
    return (l->value == r->value);
}

int compare_obf(obf_t* l, obf_t* r)
{
    return static_cast<int>((l->value < r->value) ? -(r->value - l->value) : (l->value - r->value));
}

std::size_t hash_obf(const obf_t* obf)
{
    return static_cast<std::size_t>(obf->value);
}

int main()
{
    obf_t* obf1 = build_obf(1);
    obf_t* obf2 = build_obf(2);

    assert(equal_obf(obf1, obf2) == false);
    assert(compare_obf(obf1, obf2) < 0);
    assert(compare_obf(obf2, obf1) > 0);
    assert(hash_obf(obf1) != hash_obf(obf2));

    destroy_obf(obf1);
    destroy_obf(obf2);
}
