
local fn = vim.fn
local execute = vim.api.nvim_command
local map = vim.api.nvim_set_keymap
-- disable netrw at the very start of your init.lua (strongly advised)
vim.g.loaded_netrw = 1
vim.g.loaded_netrwPlugin = 1

vim.g.airline_theme = 'deus'
--vim.cmd 'colorscheme habamax'
--vim.cmd 'colorscheme nightfox'

vim.cmd([[
  set notimeout
  set encoding=utf-8
	colorscheme one
]])

vim.opt.background = 'dark'
vim.opt.clipboard = 'unnamedplus'  -- using system clipboard
vim.opt.completeopt = 'noinsert,menuone,noselect' -- autocomplete menu
vim.opt.mouse = 'a'						  -- allow use of mouse	
vim.opt.cursorline = true			  -- highline current line
vim.opt.relativenumber = true
vim.opt.number = true			      -- show line number		
vim.opt.title = true					  -- show title file
vim.opt.wildmenu = true				  -- shows a moreadvanced menu for autocompletion suggestions
vim.opt.termguicolors = true    -- set termguicolors to enable highlight groups
vim.opt.splitright = true
--vim.opt.guifont = 'Fira Code:h10'
vim.opt.guifont = 'Iosevka Term Extended:h6'

vim.opt.expandtab = true
--vim.opt.tabstop = 2
vim.opt.shiftwidth = 4
vim.opt.foldmethod = 'syntax'

-- compile and execute

map ('n','<F4>',':!g++ -std=c++17 %<cr>',{noremap = true, silent = false})
map ('n','<F5>',':!g++ -std=c++17 % -o %:r && ./%:r<cr>',{noremap = true, silent = false})

-- ctrl-d search and replace
map ('n', '<C-d>', '*Ncgn', {noremap = true, silent = false})

-- ctrl-s => save
map ('n','<C-S>',':up<cr>',{noremap = true, silent = false})
map ('i','<C-S>','<Esc>:up<cr>',{noremap = true, silent = false})

-- moving lines up and down
map ('i','<C-Down>','<Esc>:m .+1<CR>==gi',{noremap = true, silent = false}) 
map ('i','<C-Up>','<Esc>:m .-2<CR>==gi',{noremap = true, silent = false}) 

map ('n','<C-Down>',':m .+1<CR>==',{noremap = true,silent = false}) 
map ('n','<C-Up>',':m .-2<CR>==',{noremap = true, silent = false}) 

map ('v','<C-Down>','<Esc>:m .+1<CR>gv==gv',{noremap = true, silent = false}) 
map ('v','<C-Up>','<Esc>:m .-2<CR>gv==gv',{noremap = true, silent = false})


-- Auto install packer.nvim if not exists
--local install_path = fn.stdpath('data')..'/site/pack/packer/opt/packer.nvim'
--if fn.empty(fn.glob(install_path)) > 0 then
--  execute('!git clone https://github.com/wbthomason/packer.nvim '..install_path)
--end
--vim.cmd [[packadd packer.nvim]]
--vim.cmd 'autocmd BufWritePost plugins.lua PackerCompile' -- Auto compile when there are changes in plugins.lua
--
--require("nvim-tree").setup({
--  sort_by = "case_sensitive",
--  open_on_setup = true,
--  view = {
--    mappings = {
--      list = {
--        { key = "u", action = "dir_up" },
--      },
--    },
--  },
--  renderer = {
--    group_empty = true,
--  },
--  filters = {
--    dotfiles = true,
--  },
--})

--local get_hex = require('cokeline/utils').get_hex
--
--require('cokeline').setup({
--  default_hl = {
--    fg = function(buffer)
--      return
--        buffer.is_focused
--        and get_hex('Normal', 'fg')
--         or get_hex('Comment', 'fg')
--    end,
--    bg = get_hex('ColorColumn', 'bg'),
--  },
--
--  components = {
--    {
--      text = ' ',
--      bg = get_hex('Normal', 'bg'),
--    },
--    {
--      text = '',
--      fg = get_hex('ColorColumn', 'bg'),
--      bg = get_hex('Normal', 'bg'),
--    },
--    {
--      text = function(buffer)
--        return buffer.devicon.icon
--      end,
--      fg = function(buffer)
--       return buffer.devicon.color
--      end,
--    },
--    {
--      text = ' ',
--    },
--    {
--      text = function(buffer) return buffer.filename .. '  ' end,
--      style = function(buffer)
--        return buffer.is_focused and 'bold' or nil
--      end,
--    },
--    {
--      text = '',
--      delete_buffer_on_left_click = true,
--    },
--    {
--      text = '',
--      fg = get_hex('ColorColumn', 'bg'),
--      bg = get_hex('Normal', 'bg'),
--    },
--  },
--})

--return require('packer').startup(function()
--  use {'wbthomason/packer.nvim', opt = true}
--
--  use { 
--      'nvim-telescope/telescope.nvim',
--      requires = {{'nvim-lua/popup.nvim'}, {'nvim-lua/plenary.nvim'}}
--  }
--
--  use 'EdenEast/nightfox.nvim'
--  use 'rakr/vim-one'
--
--  use 'jiangmiao/auto-pairs'
--  use 'ryanoasis/vim-devicons'
--  use 'nvim-tree/nvim-tree.lua'
--
--  use 'vim-airline/vim-airline'
--  use 'vim-airline/vim-airline-themes'
--
--  use 'nvim-treesitter/nvim-treesitter'
--
--  use({
--    'noib3/nvim-cokeline',
--    requires = 'kyazdani42/nvim-web-devicons', -- If you want devicons
--    config = function()
--      require('cokeline').setup()
--    end
--  })
--
--end)


