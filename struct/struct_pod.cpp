// Listing 4-22: A code segment illustrating structured binding declaration
// POD: plain old data
struct Result
{
    HumptyDumpty hd;
    bool success;
};
Result make_humpty()
{
    HumptyDumpty hd{};
    bool is_valid;
    // Check that hd is valid and set is_valid appropriately
    return {hd, is_valid};
}
bool send_kings_horses_and_men()
{
    auto [hd, success] = make_humpty();
    if (!success)
        return false;
    // Class invariants established
    // --snip-- 
    return true;
}