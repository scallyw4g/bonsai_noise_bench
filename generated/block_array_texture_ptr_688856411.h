// external/bonsai_stdlib/src/texture.cpp:8:0





link_internal cs
CS( texture_ptr_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal texture_ptr 
Set( texture_ptr_block_array *Arr,
  texture_ptr Element,
  texture_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  texture_ptr_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = Element;
  return *Slot;
}

link_internal void
NewBlock( texture_ptr_block_array *Arr )
{
  texture_ptr_block  *NewBlock     = Allocate( texture_ptr_block , Arr->Memory,                 1);
  texture_ptr_block **NewBlockPtrs = Allocate( texture_ptr_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( texture_ptr_block_array *Array, texture_ptr_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( texture_ptr_block_array *Array, texture_ptr_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  texture_ptr Prev = {};

  texture_ptr_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    texture_ptr E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( texture_ptr_block_array *Array, texture_ptr Element )
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

link_internal texture_ptr_block_array_index
Find( texture_ptr_block_array *Array, texture_ptr Query)
{
  texture_ptr_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(texture_ptr_block_array_index *Index)
{
  texture_ptr_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal texture_ptr 
Push( texture_ptr_block_array *Array, texture_ptr Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  texture_ptr Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal texture_ptr 
Push( texture_ptr_block_array *Array )
{
  texture_ptr Element = {};
  auto Result = Push(Array, Element);
  return Result;
}

link_internal void
Shift( texture_ptr_block_array *Array, texture_ptr Element )
{
  Assert(Array->Memory);
  texture_ptr Prev = {};

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



