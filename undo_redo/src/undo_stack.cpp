#include "dak/utility/undo_stack.h"

namespace dak::utility
{
   namespace
   {
      struct undoing
      {
         undoing(bool& f) : _f(f) { _f = true; }

         ~undoing() { _f = false; }

      private:
         bool& _f;
      };
   }

   // Create an empty undo stack.
   undo_stack::undo_stack()
      : _top(_undos.end())
   {
   }

   // Clear the undo stack.
   void undo_stack::clear()
   {
      _undos.clear();
      _top = _undos.end();

      if (changed)
         changed(*this);
   }

   // Deaden the current _top transaction data.
   void undo_stack::deaden_top()
   {
      for (auto& data : *_top)
         if (data.deaden)
            data.deaden(data.data);
   }

   // Awaken the current _top transaction data.
   void undo_stack::awaken_top() const
   {
      for (const auto& data : *_top)
         if (data.awaken)
            data.awaken(data.data);
   }

   // Commit the given modified data to the undo stack.
   // Deaden the transaction data.
   void undo_stack::commit(const transaction& tr)
   {
      // Refuse to commit during undo/redo/commit.
      if (_is_undoing)
         return;

      // If there were undone transactions, erase them now that we're commiting a new timeline.
      if (has_redo())
         _undos.erase(_top + 1, _undos.end());

      undoing undoing(_is_undoing);

      _undos.emplace_back(tr);
      _top = _undos.end() - 1;
      deaden_top();

      if (changed)
         changed(*this);
   }

   void undo_stack::simple_commit(const simple_transaction& tr)
   {
      commit({ tr });
   }

   // Undo awakens the previous transaction data. (The one before the last commit.)
   // Does nothing if at the start of the undo stack.
   void undo_stack::undo()
   {
      if (!has_undo())
         return;

      undoing undoing(_is_undoing);

      --_top;
      awaken_top();

      if (changed)
         changed(*this);
   }

   // Redo awakens the next transaction data that was commited.
   // Does nothing if at the end of the undo stack.
   void undo_stack::redo()
   {
      if (!has_redo())
         return;

      undoing undoing(_is_undoing);

      ++_top;
      awaken_top();

      if (changed)
         changed(*this);
   }
}

// vim: sw=3 : sts=3 : et : sta : 

