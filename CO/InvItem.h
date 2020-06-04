#ifndef INVITEM_H_INCLUDED
#define INVITEM_H_INCLUDED

namespace CO {
    class CInvItem
    {
    private:
        CInvItem() {}
    public:
        enum EQuality
        {
            None   = 0,
            Normal = 3,
            Refine = 6,
            Unique = 7,
            Elite  = 8,
            Super  = 9
        };

        int ItemId() const { return *(int*)((int)this + 12); }
        int UniqueId() const { return *(int*)((int)this + 4); }

        int NumSockets() const
        {
            if (*(int*)((int)this + 0x48) == 0x1F)
                return 2;
            else if (*(int*)((int)this + 0x47) == 0x1F)
                return 1;
            else
                return 0;
        }

        int NumPlus() const
        {
            return *(char*)((int)this + 0x4B);
        }

        bool IsEquip() const
        {
            return ItemId() < 0xAAE60;
        }

        EQuality Quality() const
        {
            int num = ItemId();
            if (num < 0xAAE60)
            {
                return (EQuality)(num % 10);
            }
            else
                return None;
        }
    };
}

#endif // INVITEM_H_INCLUDED
