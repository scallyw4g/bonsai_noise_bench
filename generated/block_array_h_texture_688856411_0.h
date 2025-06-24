// external/bonsai_stdlib/src/texture.h:44:0





struct texture_block
{
  /* u32 Index; */
  umm At;
  texture Elements[8];
};

struct texture_block_array_index
{
  umm Index; 
};

struct texture_block_array
{
  texture_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal texture_block_array
TextureBlockArray(memory_arena *Memory)
{
  texture_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(texture_block_array_index *Thing1, texture_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( texture_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(texture_block_array_index Thing1, texture_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( texture_block_array_index ) );

  return Result;
}


typedef texture_block_array texture_paged_list;

link_internal texture_block_array_index
operator++( texture_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( texture_block_array_index I0, texture_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( texture_block_array_index I0, texture_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( texture_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal texture_block_array_index
ZerothIndex( texture_block_array *Arr )
{
  return {};
}

link_internal texture_block_array_index
Capacity( texture_block_array *Arr )
{
  texture_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal texture_block_array_index
AtElements( texture_block_array *Arr )
{
  texture_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( texture_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal texture_block_array_index
LastIndex( texture_block_array *Arr )
{
  texture_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( texture_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal texture_block *
GetBlock( texture_block_array *Arr, texture_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  texture_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal texture *
GetPtr( texture_block_array *Arr, texture_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  texture_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  texture *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal texture *
GetPtr( texture_block_array *Arr, umm Index )
{
  texture_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal texture *
TryGetPtr( texture_block_array *Arr, texture_block_array_index Index)
{
  texture * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal texture *
TryGetPtr( texture_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, texture_block_array_index{Index});
  return Result;
}

