# 🚨 Railway Deployment Quick Fix

## The Problem

Railway is trying to deploy from the wrong directory. Your app is in `xiaozhi-mcphub-main` but Railway is deploying from the root `WRO` directory.

## ⚡ Quick Fix (2 minutes)

### Step 1: Configure Root Directory

1. Go to https://railway.app
2. Click on your **"WRO"** service
3. Click **"Settings"** tab
4. Scroll down to **"Build & Deploy"** section
5. Find **"Root Directory"** field
6. Enter: `xiaozhi-mcphub-main`
7. Click **"Save"**

### Step 2: Configure Build Settings

In the same **Settings** section:

**Build Command:**
```bash
pnpm install --frozen-lockfile && pnpm run build
```

**Start Command:**
```bash
pnpm start
```

**OR** use Dockerfile:

1. Go to **Settings** → **"Source"** section
2. Enable **"Use Docker"**
3. Set Dockerfile path: `Dockerfile.railway`

### Step 3: Add PostgreSQL Database

1. Click the **"+"** button in Railway
2. Select **"Database"** → **"Add PostgreSQL"**
3. Copy the **DATABASE_URL**
4. Go to your service → **Variables**
5. Add environment variable:
   - Key: `DATABASE_URL`
   - Value: (paste the connection string)

### Step 4: Add Other Environment Variables

In **Variables** section, add:

```
NODE_ENV=production
SMART_ROUTING_ENABLED=false
ENABLE_CORS=true
```

### Step 5: Redeploy

Click **"Deploy"** button or push to GitHub to trigger redeploy.

---

## 📸 Visual Guide

### 1. Root Directory Setting

```
Settings → Build & Deploy
┌─────────────────────────────┐
│ Root Directory:             │
│ [xiaozhi-mcphub-main     ]  │  ← Enter this
└─────────────────────────────┘
```

### 2. Build Settings

```
Settings → Build & Deploy
┌─────────────────────────────┐
│ Build Command:              │
│ [pnpm install && pnpm build]│
│                             │
│ Start Command:              │
│ [pnpm start              ]  │
└─────────────────────────────┘
```

### 3. Database Setup

```
Railway Dashboard
┌─────────────────────────────┐
│  + New                      │  ← Click this
│    ↓                        │
│  Database                    │
│    ↓                        │
│  Add PostgreSQL             │  ← Select this
└─────────────────────────────┘
```

---

## ✅ Expected Result

After saving these settings, Railway will:
1. Deploy from the correct directory (`xiaozhi-mcphub-main`)
2. Install dependencies with pnpm
3. Build the TypeScript + Vite frontend
4. Start the application
5. Connect to PostgreSQL database

---

## 🐛 Common Issues & Solutions

### Issue: Still can't find the app

**Solution:** Make sure you're setting the root directory to `xiaozhi-mcphub-main` (without trailing slash)

### Issue: Database connection error

**Solution:** 
1. Ensure PostgreSQL service is created
2. Copy the DATABASE_URL from PostgreSQL service
3. Add it to your web service variables

### Issue: Build takes too long

**Solution:** This is normal. The first build installs all dependencies and builds frontend. Subsequent builds are faster.

### Issue: "Port already in use"

**Solution:** Railway handles this automatically. No action needed.

---

## 📝 TL;DR - Copy & Paste

1. **Settings** → **Build & Deploy**
2. **Root Directory:** `xiaozhi-mcphub-main`
3. **Build Command:** `pnpm install --frozen-lockfile && pnpm run build`
4. **Start Command:** `pnpm start`
5. **Add PostgreSQL** database
6. **Add variables:** DATABASE_URL (from PostgreSQL), NODE_ENV, SMART_ROUTING_ENABLED, ENABLE_CORS
7. **Deploy**

---

## 🎉 Success!

Once deployed, your MCP Hub will be live at: `https://your-app-name.railway.app`

