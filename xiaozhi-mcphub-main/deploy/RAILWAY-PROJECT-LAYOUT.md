# 🏗️ Railway Project Layout

## ✅ Correct Layout (Same Project)

Your PostgreSQL database and web service **should be in the same Railway project**:

```
Project: WRO
├── PostgreSQL Service (Database)
│   └── Provides: DATABASE_URL, DATABASE_PUBLIC_URL
│
└── Web Service (Your App)
    └── Uses: DATABASE_URL from PostgreSQL
```

**When in same project:** Internal networking works, can use `postgres.railway.internal`

---

## ❌ Incorrect Layout (Different Projects)

If they're in different projects:

```
Project 1: Database
└── PostgreSQL Service

Project 2: Web App
└── Web Service (trying to connect to Project 1's database)
```

**When in different projects:** Internal networking doesn't work, must use **public URL**

---

## 🔧 Solution Options

### Option 1: Move Services to Same Project ✅ RECOMMENDED

**If your PostgreSQL is in a different project:**

1. Go to your PostgreSQL service
2. Click **"⋮"** (three dots) → **"Transfer Service"**
3. Select your **main WRO project**
4. Confirm transfer

**OR if it's already in the same project, continue to Option 2.**

### Option 2: Use Public URL (Works from Any Project)

If they must stay in different projects (or for external access):

1. Get the public URL from PostgreSQL service's **"Connect"** tab
2. Use this format:
   ```
   postgresql://postgres:password@containers-us-west-xxx.railway.app:5432/railway
   ```
3. Add it as `DATABASE_URL` in your web service

---

## 🎯 Quick Check: Are They in Same Project?

In Railway Dashboard:

### Same Project ✅
```
WRO (Project Name)
├── [Postgres Icon] Postgres  ← Database
└── [Globe Icon] WRO         ← Your Web Service
```

Both are under the **same project name** (WRO)

### Different Projects ❌
```
Database Project
└── [Postgres Icon] Postgres

WRO Project  
└── [Globe Icon] WRO
```

They're under **different project names**

---

## 🔄 How to Move Services to Same Project

### Step 1: Transfer PostgreSQL Service

1. Go to your PostgreSQL service (in Database project)
2. Click **"⋮"** (three dots) at top right
3. Click **"Settings"**
4. Scroll to **"Transfer Service"** section
5. Select **"WRO"** project from dropdown
6. Click **"Transfer"**
7. Confirm

### Step 2: Verify

After transfer:
1. Both services should now show under **"WRO"** project
2. PostgreSQL internal networking should work
3. Update DATABASE_URL to use public URL if needed

---

## 💡 Best Practice

**For Production:**
- ✅ Keep database and web service in **same project**
- ✅ Use internal service discovery (automatic DNS)
- ✅ Better security (no public exposure)
- ✅ Better performance (internal networking)

**For Development:**
- Can use public URLs if needed
- Works across projects
- Easier to share with external services

---

## 🚨 Current Issue

Based on your error (`postgres.railway.internal` not found):

**Either:**
1. Services are in different projects → Move to same project **OR** use public URL
2. Services are in same project but internal DNS isn't working → Use public URL

**Quick Fix:** Use the public URL from PostgreSQL's "Connect" tab and add it as `DATABASE_URL`.

