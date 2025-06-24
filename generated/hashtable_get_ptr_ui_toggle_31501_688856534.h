// external/bonsai_stdlib/src/ui/ui.cpp:16:0

maybe_ui_toggle_ptr
GetPtrById( ui_toggle_hashtable *Table, ui_id Query )
{
  /* ENSURE_OWNED_BY_THREAD(Table); */

  maybe_ui_toggle_ptr Result = {};

  auto *Bucket = GetHashBucket(umm(Hash(&Query)), Table);
  while (Bucket)
  {
    auto E = &Bucket->Element;

    if (Bucket->Tombstoned == False && AreEqual(E->Id, Query))
    {
      Result.Tag = Maybe_Yes;
      Result.Value = E;
      break;
    }
    else
    {
      Bucket = Bucket->Next;
    }
  }

  return Result;


}

