// external/bonsai_stdlib/src/shader.cpp:4:0





link_internal cs
CS( shader_ptr_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal shader_ptr 
Set( shader_ptr_block_array *Arr,
  shader_ptr Element,
  shader_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  shader_ptr_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 64;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = Element;
  return *Slot;
}

link_internal void
NewBlock( shader_ptr_block_array *Arr )
{
  shader_ptr_block  *NewBlock     = Allocate( shader_ptr_block , Arr->Memory,                 1);
  shader_ptr_block **NewBlockPtrs = Allocate( shader_ptr_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( shader_ptr_block_array *Array, shader_ptr_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( shader_ptr_block_array *Array, shader_ptr_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  shader_ptr Prev = {};

  shader_ptr_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    shader_ptr E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( shader_ptr_block_array *Array, shader_ptr Element )
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

link_internal shader_ptr_block_array_index
Find( shader_ptr_block_array *Array, shader_ptr Query)
{
  shader_ptr_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(shader_ptr_block_array_index *Index)
{
  shader_ptr_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal shader_ptr 
Push( shader_ptr_block_array *Array, shader_ptr Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  shader_ptr Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal shader_ptr 
Push( shader_ptr_block_array *Array )
{
  shader_ptr Element = {};
  auto Result = Push(Array, Element);
  return Result;
}

link_internal void
Shift( shader_ptr_block_array *Array, shader_ptr Element )
{
  Assert(Array->Memory);
  shader_ptr Prev = {};

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



