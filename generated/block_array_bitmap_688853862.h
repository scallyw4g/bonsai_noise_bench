// external/bonsai_stdlib/src/bitmap.cpp:182:0





struct bitmap_block
{
  /* u32 Index; */
  umm At;
  bitmap Elements[8];
};

struct bitmap_block_array_index
{
  umm Index; 
};

struct bitmap_block_array
{
  bitmap_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal bitmap_block_array
BitmapBlockArray(memory_arena *Memory)
{
  bitmap_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(bitmap_block_array_index *Thing1, bitmap_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( bitmap_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(bitmap_block_array_index Thing1, bitmap_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( bitmap_block_array_index ) );

  return Result;
}


typedef bitmap_block_array bitmap_paged_list;

link_internal bitmap_block_array_index
operator++( bitmap_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( bitmap_block_array_index I0, bitmap_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( bitmap_block_array_index I0, bitmap_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( bitmap_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal bitmap_block_array_index
ZerothIndex( bitmap_block_array *Arr )
{
  return {};
}

link_internal bitmap_block_array_index
Capacity( bitmap_block_array *Arr )
{
  bitmap_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal bitmap_block_array_index
AtElements( bitmap_block_array *Arr )
{
  bitmap_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( bitmap_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal bitmap_block_array_index
LastIndex( bitmap_block_array *Arr )
{
  bitmap_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( bitmap_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal bitmap_block *
GetBlock( bitmap_block_array *Arr, bitmap_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  bitmap_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal bitmap *
GetPtr( bitmap_block_array *Arr, bitmap_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  bitmap_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  bitmap *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal bitmap *
GetPtr( bitmap_block_array *Arr, umm Index )
{
  bitmap_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal bitmap *
TryGetPtr( bitmap_block_array *Arr, bitmap_block_array_index Index)
{
  bitmap * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal bitmap *
TryGetPtr( bitmap_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, bitmap_block_array_index{Index});
  return Result;
}





link_internal cs
CS( bitmap_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal bitmap *
Set( bitmap_block_array *Arr,
  bitmap *Element,
  bitmap_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  bitmap_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( bitmap_block_array *Arr )
{
  bitmap_block  *NewBlock     = Allocate( bitmap_block , Arr->Memory,                 1);
  bitmap_block **NewBlockPtrs = Allocate( bitmap_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( bitmap_block_array *Array, bitmap_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( bitmap_block_array *Array, bitmap_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  bitmap *Prev = {};

  bitmap_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    bitmap *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( bitmap_block_array *Array, bitmap *Element )
{
  IterateOver(Array, E, I)
  {
    if (E == Element)
    {
      RemoveOrdered(Array, I);
      break;
    }
  }
}

link_internal bitmap_block_array_index
Find( bitmap_block_array *Array, bitmap *Query)
{
  bitmap_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
  IterateOver(Array, E, Index)
  {
    if ( E == Query )
    {
      Result = Index;
      break;
    }
  }
  return Result;
}



link_internal b32
IsValid(bitmap_block_array_index *Index)
{
  bitmap_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal bitmap *
Push( bitmap_block_array *Array, bitmap *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  bitmap *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal bitmap *
Push( bitmap_block_array *Array )
{
  bitmap Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Shift( bitmap_block_array *Array, bitmap *Element )
{
  Assert(Array->Memory);
  bitmap *Prev = {};

  // Alocate a new thingy
  Push(Array);

  auto End = AtElements(Array);
  RangeIteratorReverse(Index, s32(End.Index))
  {
    auto E = GetPtr(Array, umm(Index));
    if (Prev) { *Prev = *E; }
    Prev = E;
  }

  *Prev = *Element;
}




